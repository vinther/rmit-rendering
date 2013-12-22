from argparse import ArgumentParser
import os
import re
import mmh3

parser = ArgumentParser()
parser.add_argument("input_directories", nargs='+')
parser.add_argument("-v", "--verbose", action="store_true")
options = parser.parse_args()

if __name__ == "__main__":
    file_names = []

    for input_directory in options.input_directories:
        for base, _, files in os.walk(input_directory):
            if not len(files):
                continue

            rel_base = os.path.relpath(base, input_directory)

            for f in files:
                file_names.append((base, f))

    p = re.compile('hash\("(.+)"(?:,\s*(-?[0-9a-fx]+)+\s*)?\)')

    results = []

    for (base, rel_file_name) in file_names:
        file_path = os.path.join(base, rel_file_name)

        replacement = None

        with open(file_path, 'r') as f:
            contents = f.read()

            matches = p.findall(contents)
            num_replacements = 0

            for (fp, old_hash) in matches:
                hash = mmh3.hash64(fp)[0]
                old_hash = int(old_hash, 16) if old_hash else 0

                results.append({
                    'base': base,
                    'rel_file_name': rel_file_name,
                    'path': fp,
                    'hash': hash,
                    'old_hash': old_hash,
                })

                if hash != old_hash:
                    num_replacements += 1

            def repl(x):
                g = x.groups()

                return 'hash("%s", %#x)' % (g[0], mmh3.hash64(g[0])[0])

            if num_replacements > 0:
                replacement = p.sub(repl, contents)

        if replacement is not None:
            with open(file_path, 'w') as f:
                f.write(replacement)

    unique_crc = {}

    for res in results:
        if res['hash'] in unique_crc and res['path'] != unique_crc[res['hash']]:
            print "MMH3 hash collision: '%s' (%d) and '%s' (%d)" % (res['path'], res['hash'], unique_crc[res['hash']], res['hash'])
        else:
            unique_crc[res['hash']] = res['path']

    num_replaced = sum([1 if res['hash'] != res['old_hash'] else 0 for res in results])

    if options.verbose:
        for res in results:
            print "%s\t%s\t%s\t%s\t%#x\t%#x" % (
                'Y' if res['hash'] != res['old_hash'] else 'N',
                res['base'],
                res['rel_file_name'],
                res['path'],
                res['hash'],
                res['old_hash'],
            )

        print "Replaced %d hashes!" % num_replaced
