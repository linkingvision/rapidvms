icu.exe is a self-extract pre-built (by us) IBM ICU 52.1 library.

The sources was downloaded from http://site.icu-project.org/download.

Two customized data files was created using the tool at http://apps.icu-project.org/datacustom/ICUData52.html.

One was created empty and saved to icudt52l_empty.dat. This file is used when building ICU, to
make icudt a little stub. The file was saved in ICU sources directory at <icu>\source\data\in
with name icudt52l.dat, before start ICU building process.

The other is the data file we use in runtime. It was created selecting the options:
- Charset Mapping Tables:
    gb18030
    ibm-1386_P100-2001
    ibm-942_P12A-1999
    ibm-943_P130-1999
    ibm-943_P15A-2003
    icu-internal-25546
    windows-874-2000
    windows-936-2000

- Collators: all

- Base data: all
