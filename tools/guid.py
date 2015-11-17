import os
import re
import sys

from argparse import ArgumentParser
from cStringIO import StringIO

DELPHI_GUID_PATTERN = "\s+SID_(\w+)\s+=\s+'{(\w+)-(\w+)-(\w+)-(\w{2})(\w{2})-(\w{2})(\w{2})(\w{2})(\w{2})(\w{2})(\w{2})}';"
CPP_GUID_PATTERN = "\w+ IID_(\w+)\s+=\s+{0x(\w+), 0x(\w+), 0x(\w+), 0x(\w{2}), 0x(\w{2}), 0x(\w{2}), 0x(\w{2}), 0x(\w{2}), 0x(\w{2}), 0x(\w{2}), 0x(\w{2})};"

GUID_FORMAT_CPP = "static const GUID IID_{0} = {{0x{1}, 0x{2}, 0x{3}, 0x{4}, 0x{5}, 0x{6}, 0x{7}, 0x{8}, 0x{9}, 0x{10}, 0x{11}}};"
GUID_FORMAT_DELPHI = "  SID_{0} = '{{{1}-{2}-{3}-{4}{5}-{6}{7}{8}{9}{10}{11}}}';"

H_TEMPLATE = '${DECLSPEC_SECTION}\n\n${SECTION_DEFINE}'
H_TEMPLATE_FILENAME = 'comtypes.h.template'
H_FILENAME = 'comtypes.h'

# SECTION_SID = '${SECTION_SID}'
SECTION_DECLSPEC = '${SECTION_DECLSPEC}'
SECTION_DEFINE = '${SECTION_DEFINE}'

# SECTION_SID_LINE = '#define SID_{0} "{1}-{2}-{3}-{4}{5}-{6}{7}{8}{9}{10}{11}"'
SECTION_DECLSPEC_LINE = 'class DECLSPEC_UUID("{1}-{2}-{3}-{4}{5}-{6}{7}{8}{9}{10}{11}") {0};'
SECTION_TYPEDEF_LINE = '#define {0}Ptr COM_PTR_T({0})'

EOF = '\n'

g_sdk_dir = None

def print_guids_in_format(guids, fmt):
    for guid in sorted(guids):
        print(fmt.format(*guid))


def print_cpp_guids():
    print_guids_in_format(get_cpp_guids(), GUID_FORMAT_CPP)


def print_delph_guids():
    print_guids_in_format(get_cpp_guids(), GUID_FORMAT_DELPHI)


def get_guids(directory, guid_pattern):
    pattern = re.compile(guid_pattern)
    guids = []

    for root, dirs, files in os.walk(directory):
        for fn in files:
            fp = os.path.join(root, fn)
            with open(fp, 'r') as fd:
                data = fd.read()
                guids.extend(pattern.findall(data))

    return guids


def get_cpp_guids():
    sources_path = os.path.join(g_sdk_dir, 'Sources', 'Cpp')
    return get_guids(sources_path, CPP_GUID_PATTERN)


def get_delphi_guids():
    sources_path = os.path.join(g_sdk_dir, 'Sources', 'Delphi')
    return get_guids(sources_path, DELPHI_GUID_PATTERN)


def print_diff_guids():
    c_guids = get_cpp_guids()
    d_guids = get_delphi_guids()
    diff_guids = list(set(c_guids) - set(d_guids))
    diff_guids.sort()
    if diff_guids:
        for guid in diff_guids:
            print(GUID_FORMAT_CPP.format(*guid))
            print(GUID_FORMAT_DELPHI.format(*guid))
    else:
        print 'All interface GUIDs are equal'


def generate_com_helpers_h():
    print('Generating {0}...'.format(H_FILENAME))
    if os.path.exists(H_TEMPLATE_FILENAME):
        with open(H_TEMPLATE_FILENAME, 'r') as fd:
            h_template = fd.read()
    else:
        print('User file template is missing, use default one'.format(H_FILENAME))
        h_template = H_TEMPLATE[:]
        with open(H_TEMPLATE_FILENAME, 'w') as fd:
            fd.write(H_TEMPLATE)

    guids = get_cpp_guids()
    guids.sort()

    # format: [buf to write][, [var in template], [guid line template], [lines before guids], [lines after guids]
    sections = (
        (
            StringIO(),
            SECTION_DECLSPEC,
            SECTION_DECLSPEC_LINE,
            (
                '// IUnknown UUID',
                'class DECLSPEC_UUID("00000000-0000-0000-C000-000000000046") IAIMPPlugin;\n',
            ),
            ( ),
        ),
        (
            StringIO(),
            SECTION_DEFINE,
            SECTION_TYPEDEF_LINE,
            (
                '#define COM_PTR_T(x) _com_ptr_t<_com_IIID<x,  &__uuidof(x)>>',
            ),
            ( ),
        ),
    )

    for section, var, fmt, pref, suf in sections:
        buf = []
        buf.extend(pref)
        buf.extend(fmt.format(*guid) for guid in guids)
        buf.extend(suf)
        for line in buf:
            section.write(line)
            section.write(EOF)
        h_template = h_template.replace(var, section.getvalue())
        section.close()

    with open(H_FILENAME, 'w') as fd:
        fd.write(h_template)
    print('Done')


def is_sdk_dir(sdk_dir):
    return os.path.exists(sdk_dir)


def main():
    parser = ArgumentParser()
    parser.add_argument('--sdk', dest='sdk_dir', default='.', required=False, help='AIMP SDK path')
    group = parser.add_mutually_exclusive_group(required=True)
    group.add_argument('--gen', dest='generate', action='store_true', help='generate comtypes.h')
    group.add_argument('--cpp', dest='printcpp', action='store_true', help='print cpp guid list')
    group.add_argument('--diff', dest='diff', action='store_true', help='print different guids')
    args = parser.parse_args()

    if not is_sdk_dir(args.sdk_dir):
        print('Invalid SDK directory path')
        return 1

    global g_sdk_dir
    g_sdk_dir = args.sdk_dir

    if args.generate:
        generate_com_helpers_h()
    elif args.printcpp:
        print_cpp_guids()
    elif args.diff:
        print_diff_guids()
    return 0


if '__main__' == __name__:
    sys.exit(main())
