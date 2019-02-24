#!/usr/bin/env python3

import colorednoise


def normalize(array, max_int):
    n_min, n_max = min(array), max(array)
    spread = n_max - n_min
    out = []
    for v in array:
        out.append(int((v - n_min) / spread * max_int))
    return out
    # print(n_min, n_max)


def hexify(array):
    return [hex(v) for v in array]


def main():
    noise = colorednoise.powerlaw_psd_gaussian(1, 256)
    noise = normalize(noise, 255)
    print(', '.join(hexify(noise)))


if __name__ == '__main__':
    main()
