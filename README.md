# Introduction

This is a tool for conversion of Chinese characters
(traditional/simplified) in `docx` files
based on [OpenCC](https://github.com/BYVoid/OpenCC).

It does not only do the conversion, but also generates an `html` file
containing marks on characters which have alternative forms in the
conversion.

# Setup

1. Clone this repo.
1. Download `opencc.dll` 
    [here](https://github.com/nemo-nullius/OpenCC/releases)
    and put it in `py/clib/bin/`. 
    **NOTE: this release is modified based on [OpenCC](https://github.com/BYVoid/OpenCC),
    and the modification can be seen [here](https://github.com/BYVoid/OpenCC/compare/master...nemo-nullius:master).
    It is compiled for Windows 10 64bit version.
    Please download the source code and compile it for other platforms.**
1. Download [config files](https://github.com/BYVoid/OpenCC/tree/master/data/config)
   and [dictionaries](https://github.com/BYVoid/OpenCC/tree/master/data/dictionary),
   and put them all in `py/clib/share/opencc/`.
   (For better performance, `dictionaries` could be compiled into `ocd2` format
   and be put in the previous folder. Also they can be found in `opencc` package
   for python.)
1. Create your own `config files` or `dictionaries` if you wish.

# Usage

```bash
python docx_char_cnv.py <filename-for-conversion> <config>
# for example
python docx_char_cnv.py ./trad.docx t2s.json
python docx_char_cnv.py ./simp.docx ./my_config_file.json
```

The converted file will have a `_modified` suffix after 
`filename-for-conversion` in the same folder. 

An `html` file with the same filename will also be generated, in which
all characters having alternative forms in the conversion
will be marked with red color and a "◣" symbol behind.

As for the configuration of customized config files, please refer to
files in `sample` folder.

# Features

- Directly perform conversion on `docx` files.
- Customized dictionaries and rules for conversion (thanks to `OpenCC`).
- Accurate and quick conversion (thanks to `OpenCC`).
- All original formats in `docx` will be kept after conversion.
- Make it easy for proofreading after conversion.

# Acknowledgements

- [OpenCC](https://github.com/BYVoid/OpenCC) Apache-2.0 License
   