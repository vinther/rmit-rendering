from argparse import ArgumentParser
import json
import os
import lzo
import mmap

parser = ArgumentParser()
parser.add_argument("input")
parser.add_argument("-o", "--output", action="store", default="output")
parser.add_argument("--mmap", action="store_true", default=False)
options = parser.parse_args()

if __name__ == "__main__":
    file_descriptors = json.loads(open(options.input + '.json', 'r+b').read())
    compressed_files = open(options.input + '.pak', 'r+b')

    if options.mmap:
        mm = mmap.mmap(compressed_files.fileno(), 0)

    for f in file_descriptors['files']:
        path = f['path']
        output_file_dir = os.path.join(options.output, os.path.dirname(path))

        if not os.path.exists(output_file_dir):
            os.makedirs(output_file_dir)

        start = f['begin']
        size = f['size']

        if options.mmap:
            mm.seek(start)
            intermediate_contents = mm.read(size)

            if f['compression']:
                contents = lzo.decompress(intermediate_contents)
            else:
                contents = intermediate_contents
        else:
            compressed_files.seek(start)
            intermediate_contents = compressed_files.read(size)

            if f['compression']:
                contents = lzo.decompress(intermediate_contents)
            else:
                contents = intermediate_contents

        open(os.path.join(options.output, path), 'wb').write(contents)

