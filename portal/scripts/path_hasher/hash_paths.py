from argparse import ArgumentParser
import os
import re
import hashlib
from zlib import crc32

from jinja2 import Template

parser = ArgumentParser()
parser.add_argument("input_directory")
parser.add_argument("-cut", "--cutoff", action="store", type=float, default=0.9)
parser.add_argument("-c", "--compression", action="store", type=int, choices=range(1, 10), default=9)
parser.add_argument("-v", "--verbose", action="store_true")
parser.add_argument("-j", "--workers", action="store", type=int, default=1)
parser.add_argument("-o", "--output", action="store", default="./out")
parser.add_argument("-t", "--template", action="store", default="file_hashes.hppt")
options = parser.parse_args()

if __name__ == "__main__":

    file_names = []

    for base, _, files in os.walk(options.input_directory):
        if not len(files):
            continue

        rel_base = os.path.relpath(base, options.input_directory)

        for f in files:
            file_names.append((base, f))

    p = re.compile('CT_FILE_PATH\(\s*"(.+)"\s*,\s*([0-9])+\s*\)')

    results = []

    for (base, rel_file_name) in file_names:
        file_path = os.path.join(base, rel_file_name)

        replacement = None

        with open(file_path, 'r') as f:
            contents = f.read()

            ct_file_paths = p.findall(contents)

            for (fp, old_hash) in ct_file_paths:
                results.append({
                    'base': base,
                    'rel_file_name': rel_file_name,
                    'path': fp,
                    'old_hash': int(old_hash),
                    'hash': crc32(fp),
                })

            def repl(x):
                g = x.groups()

                return 'CT_FILE_PATH("{0}", {1})'.format(g[0], crc32(g[0]))

            replacement = p.sub(repl, contents)
            
        if replacement is not None:
            with open(file_path, 'w') as f:
                f.write(replacement)

    print results

    template = Template(open(options.template, 'r').read())
    #print template.render(results=results)


