from __future__ import print_function
from Queue import Queue
from argparse import ArgumentParser
import json
import os
import lzo
from threading import Thread
import sys
import hashlib

print = lambda x: sys.stdout.write("%s\n" % x)

parser = ArgumentParser()
parser.add_argument("input_directory")
parser.add_argument("-cut", "--cutoff", action="store", type=float, default=0.9)
parser.add_argument("-c", "--compression", action="store", type=int, choices=range(1, 10), default=9)
parser.add_argument("-v", "--verbose", action="store_true")
parser.add_argument("-j", "--workers", action="store", type=int, default=1)
parser.add_argument("-o", "--output", action="store", default="./out")
options = parser.parse_args()

extensions = {
    'unknown': 0,
    'txt': 1,
    'obj': 2,
    'mtl': 3,
    'jpg': 4, 'jpeg': 4,
    'tga': 5,
    'glsl': 6,
}

q = Queue()
compressed_data_chunks = []


def compressor_worker():
    while not q.empty():
        w_base, w_rel_base, w_f = q.get()

        w_rel_base = '' if w_rel_base == '.' else w_rel_base

        abs_path = os.path.join(w_base, w_f)
        rel_path = os.path.join(w_rel_base, w_f)

        extension = os.path.splitext(rel_path)[1][1:]

        raw_filestring = open(abs_path).read()
        compressed_filestring = lzo.compress(raw_filestring, options.compression)

        len_raw = len(raw_filestring)
        len_compressed = len(compressed_filestring)

        compression_factor = (float(len_compressed) / len_raw) if len_raw else 0
        compression_used = False

        if compression_factor < options.cutoff:
            compression_used = True

        len_final = len_compressed if compression_used else len_raw

        compressed_data_chunks.append({
            'path': rel_path,
            'path_hash': hashlib.md5(rel_path).hexdigest(),
            'crc32': lzo.crc32(raw_filestring),
            'adler32': lzo.adler32(raw_filestring),
            'size': len_final,
            'original_size': len_raw,
            'factor': compression_factor,
            'compression': 1 if compression_used else 0,
            'extension_str': extension,
            'extension': extensions[extension] if extension in extensions else 0,
            'data': compressed_filestring if compression_used else raw_filestring
        })

        if options.verbose:
            print('\t'.join((
                'Y' if compression_used else 'N',
                extension,
                '%.02f' % (compression_factor * 100.0),
                str(len_raw / 1024),
                str(len_final / 1024),
                rel_path
            )))

        q.task_done()


if __name__ == "__main__":
    for base, _, files in os.walk(options.input_directory):
        if not len(files):
            continue

        rel_base = os.path.relpath(base, options.input_directory)

        for f in files:
            q.put((base, rel_base, f))

    for i in range(options.workers):
        t = Thread(target=compressor_worker)
        t.daemon = True
        t.start()

    q.join()

    num_bytes = 0
    num_compressed_bytes = 0

    file_data = []
    file_descriptors = []

    for c in sorted(compressed_data_chunks, key=lambda x: x['path']):
        descriptor = {'begin': num_compressed_bytes}

        for item in ('path', 'path_hash', 'crc32', 'adler32', 'compression', 'extension', 'original_size', 'size'):
            descriptor[item] = c[item]

        file_descriptors.append(descriptor)

        file_data.append(c['data'])

        num_bytes += c['original_size']
        num_compressed_bytes += c['size']

    combined_data = ''.join(file_data)

    file_descriptor_path = options.output + '.json'
    file_data_path = options.output + '.pak'

    output = {
        'version': 2,
        'file_descriptors': file_descriptors,
        'data_file_path': file_data_path,
        'size': len(combined_data),
        'num_files': len(file_descriptors),
        'crc32': lzo.crc32(combined_data),
        'adler32': lzo.adler32(combined_data)
    }

    data_json = json.dumps(output)

    open(file_descriptor_path, "w").write(data_json)
    open(file_data_path, "w").write(combined_data)

    if options.verbose:
        print("Total: %d bytes -> %d bytes (%.02f%%)" % (num_bytes, num_compressed_bytes, (float(num_compressed_bytes) / num_bytes) * 100.0))