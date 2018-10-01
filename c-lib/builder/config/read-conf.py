"""

Read the default configuration.

Copyright (C) 2018, Guillaume Gonnet
This project is under the MIT License

"""

import os, os.path as path
import sys
import argparse
import configparser
import json
import re


# Regexps
_re_comments = re.compile("^((?:[^\"]*(?:\"[^\"]*\")?)*)#.*$")
_re_int = re.compile("^[0-9]+$")
_re_str = re.compile("^\"[0-9a-z ._-]*\"$", re.I)
_re_bool = re.compile('^(true|false)$', re.I)

# Have we an error?
_has_error = False



def error(msg):
    "Print an error."

    global _has_error
    _has_error = True

    print("\033[31m%s\033[0m" % msg)



def load_config(filename):
    "Load an INI configuration file."

    config = configparser.ConfigParser()
    with open(filename) as file:
        config.read_string("[DEFAULT]\n%s" % file.read())

    return config["DEFAULT"]



def load_keys(filename):
    "Load the list of keys"

    with open(filename) as file:
        data = json.load(file)

    keys = {}
    for tname, array in data.items():
        keys.update({ k: tname for k in array })

    return keys



def parse_bool(tname, value, name):
    "Parse a boolean value"

    if tname == "string":
        return error("Invalid type %s for parameter '%s'" % (tname, name))
    if tname == "boolean":
        return ("0", "1")[value == "true"]

    real = int(value)
    if not 0 <= real <= 1:
        return error("Int value must be 0 or 1 for boolean parameter '%s'" % name)

    return value



def check_value(value, expected, name):
    "Check and parse a value."

    value = _re_comments.sub("\\1", value, 1).strip()

    types = {"string": _re_str, "integer": _re_int, "boolean": _re_bool}
    for tname, regexp in types.items():
        if regexp.match(value):
            break
    else:
        return error("Invalid value '%s' for parameter %s" % (value, name))

    if expected == "boolean":
        return parse_bool(tname, value, name)
    elif tname != expected: 
        return error("Invalid type '%s' for parameter '%s'" % (tname, name))

    return value



def add_entry(key, value, keys, file):
    "Add a new entry in the header file."

    if key not in keys:
        return error("Parameter '%s' doesn't exist" % key)

    value = check_value(value, keys[key], key)
    if not value: return

    rkey = key.upper().replace("-", "_")
    del keys[key]

    file.write("#define _CTYCONF_%s %s\n\n" % (rkey, value))



def main():
    "Entry point of this script."

    parser = argparse.ArgumentParser(prog="read-conf",
        description="Read the default configuration.")

    parser.add_argument("config", default="default-conf.ini", nargs="?",
        help="defaulr configuration file.")

    parser.add_argument("-o", default="include/private/config-values.h",
        nargs="?", help="output header file.")

    args = parser.parse_args()

    here = path.dirname(__file__)
    root = path.join(here, "../..")


    config = load_config(path.join(root, args.config))
    keys = load_keys(path.join(here, "keys.json"))

    file = open(path.join(root, args.o), "w")
    with open(path.join(here, "base-header.h")) as base:
        parts = base.read().split("//%content")

    file.write("%s\n\n\n" % parts[0].strip())
    for key, value in config.items():
        add_entry(key, value, keys, file)

    if len(keys) != 0:
        error("Some parameters are not set, please provide them.")
    if not _has_error:
        file.write("\n%s\n" % parts[1].strip())

    file.close()
    if _has_error:
        os.unlink(file.name)
        sys.exit(1)


if __name__ == "__main__":
    config = main()
