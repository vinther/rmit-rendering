from __future__ import print_function
from Queue import Queue
from argparse import ArgumentParser
import json
import os
import lzo
from threading import Thread
import sys
import mmh3

print_old = print
print = lambda x: sys.stdout.write("%s\n" % x)

parser = ArgumentParser()
parser.add_argument("input_directory")
parser.add_argument("-cut", "--cutoff", action="store", type=float, default=0.9)
parser.add_argument("-c", "--compression", action="store", type=int, choices=range(1, 10), default=9)
parser.add_argument("-v", "--verbose", action="store_true")
parser.add_argument("-j", "--workers", action="store", type=int, default=1)
parser.add_argument("-o", "--output", action="store", default="./out.json")
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

        if compression_factor < options.cutoff and False:
            compression_used = True

        string_final = compressed_filestring if compression_used else raw_filestring
        len_final = len(string_final)
        adler32_final = lzo.adler32(string_final)

        compressed_data_chunks.append({
            'path': rel_path,
            'path_mmh3': mmh3.hash64(rel_path)[0],
            'adler32': adler32_final,
            'size_before': len_raw,
            'size_after': len_final,
            'factor': compression_factor,
            'compression': 1 if compression_used else 0,
            'extension_str': extension,
            'extension': extensions[extension] if extension in extensions else 0,
            'data': string_final
        })

        if options.verbose:
            print('\t'.join((
                'Y' if compression_used else 'N',
                extension,
                '%.02f' % (compression_factor * 100.0),
                str(len_raw / 1024),
                str(len_final / 1024),
                str(adler32_final),
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

        for item in ('path', 'path_mmh3', 'adler32', 'compression', 'extension', 'size_before', 'size_after'):
            descriptor[item] = c[item]

        file_descriptors.append(descriptor)

        file_data.append(c['data'])

        num_bytes += c['size_before']
        num_compressed_bytes += c['size_after']

    combined_data = ''.join(file_data)

    file_descriptors_path = options.output
    file_data_path = os.path.splitext(options.output)[0] + '.pak'

    output = {
        'version': 2,
        'file_descriptors': file_descriptors,
        'data_file_path': os.path.basename(file_data_path),
        'descriptors_file_path': os.path.basename(file_descriptors_path),
        'size': len(combined_data),
        'num_files': len(file_descriptors),
        'crc32': lzo.crc32(combined_data),
        'adler32': lzo.adler32(combined_data)
    }

    data_json = json.dumps(output)

    open(file_data_path, "w").write(combined_data)
    open(file_descriptors_path, "w").write(data_json)

    if options.verbose:
        print("Total: %d bytes -> %d bytes (%.02f%%)" % (num_bytes, num_compressed_bytes, (float(num_compressed_bytes) / num_bytes) * 100.0))