from sys import argv


class Pixel:
    def __init__(self, red, green, blue):
        self.red = red
        self.green = green
        self.blue = blue

    def __add__(self, other):
        return Pixel(
            self.red + other.red, self.green + other.green, self.blue + other.blue
        )

    def __sub__(self, other):
        return Pixel(
            self.red - other.red, self.green - other.green, self.blue - other.blue
        )

    def __mul__(self, number):
        return Pixel(self.red * number, self.green * number, self.blue * number)

    def __div__(self, number):
        return Pixel(self.red / number, self.green / number, self.blue / number)

    def __floordiv__(self, number):
        return Pixel(self.red // number, self.green // number, self.blue // number)

    def __mod__(self, number):
        return Pixel(self.red % number, self.green % number, self.blue % number)

    def uniform_quantization(self, step):
        r = int(self.red // step * step)
        g = int(self.green // step * step)
        b = int(self.blue // step * step)
        return Pixel(r, g, b)


class PixelBitmap:
    def __init__(self, bitmap, width, height):
        self.width = width
        self.height = height

        result = []
        row = []
        for i in range(width * height):
            row.append(
                Pixel(
                    blue=bitmap[i * 3], green=bitmap[i * 3 + 1], red=bitmap[i * 3 + 2]
                )
            )

            if width == len(row):
                result.insert(0, row)
                row = []
        self.bitmap = result

    def __getitem__(self, pos): #return pixel
        x, y = pos
        ret_x, ret_y = x, y
        if x < 0:
            ret_x = 0
        elif x >= self.width:
            ret_x = self.width - 1

        if y < 0:
            ret_y = 0
        elif y >= self.height:
            ret_y = self.height - 1

        return self.bitmap[ret_y][ret_x]


def filters(bitmap):
    fil_low = []
    fil_high = []
    prev = Pixel(0, 0, 0)
    for y in reversed(range(bitmap.height)):
        for x in range(bitmap.width):
            r = bitmap[x, y].red
            g = bitmap[x, y].green
            b = bitmap[x, y].blue

            yn = Pixel(int(r/2 + prev.red/2), int(g/2 + prev.green/2), int(b/2 + prev.blue/2)) #średnia z pikseli
            zn = Pixel(max(r - yn.red, 0), max(g - yn.green, 0), max(b - yn.blue, 0)) #odchylenie

            fil_low.append(yn)
            fil_high.append(zn)
            prev = bitmap[x, y]

    fil_low_trunc = []
    fil_high_trunc = []
    for i in range(0, len(fil_low), 2):     # używamy pomniejszonych filtrów (zapisujemy co drugi element filtra)
        fil_low_trunc.append(fil_low[i])
        fil_high_trunc.append(fil_high[i])

    return fil_low_trunc, fil_high_trunc


def bitmap_to_array(bitmap):
    # print(type(bitmap))
    array = []
    for pixel in bitmap:
        array += [(pixel.blue + 256) % 256, (pixel.green + 256) % 256, (pixel.red + 256) % 256]
    return array


def differential_coding(bitmap):    # kodowanie różnicowe
    a = bitmap[0]
    result = [a]
    for pixel in bitmap[1:]:
        a = pixel - a
        result.append(a)
        a = pixel

    return result


def differential_decoding(diffs):   # dekodowanie różnicowe
    a = diffs[0]
    result = [a]
    for x in diffs[1:]:
        a = a + x
        result.append(a)
    return result


def low_high_decoding(low, high):   # odwracanie filtrów
    decoded = []

    for i in range(len(low)):
        decoded.append(low[i] - high[i])
        decoded.append(low[i] + high[i])

    result = []
    for i in range(len(decoded)):
        result += ([(decoded[i].red + 256) % 256, (decoded[i].green + 256) % 256, (decoded[i].blue + 256) % 256])
    return result


def quantify_uniform(bitmap, k):    # liczy rozkład jednostajny
    step = 256 // (2 ** k)
    return [pixel.uniform_quantization(step) for pixel in bitmap]


def encode(bitmap, k):
    filtered_low, filtered_high = filters(bitmap)

    low = differential_coding(filtered_low)
    low_bytes = bytes(bitmap_to_array(low))
    quantified = quantify_uniform(filtered_high, k)
    high_bytes = bytes(bitmap_to_array(quantified))

    # spłaszczamy bitmapę
    bitmap = [
        bitmap[x, y]
        for y in reversed(range(bitmap.height))
        for x in range(bitmap.width)
    ]

    return low_bytes, high_bytes


def to_array_pixels(org):
    result = []
    for i in range(0, len(org), 3):
        result.append(Pixel(org[i], org[i + 1], org[i + 2]))
    return result


def decode(low, high, width, height):

    low = to_array_pixels(low)
    high = to_array_pixels(high)
    low_decoded = differential_decoding(low)
    decoded = low_high_decoding(low_decoded, high)
    return bytes(decoded)


def d(a, b):
    return (a - b) ** 2


def mse(original, new):
    # return (1 / len(original)) * sum([d(a, b) for a, b in zip(original, new)])
    return sum([d(a, b) for a, b in zip(original, new)]) / len(original)


def snr(x, mserr):
    return ((1 / len(x)) * sum([d(i, 0) for i in x])) / mserr


def tests(original, new):
    original_array = []
    for pixel in original:
        original_array += [pixel.blue, pixel.green, pixel.red]

    original_red = [pixel.red for pixel in original]
    original_green = [pixel.green for pixel in original]
    original_blue = [pixel.blue for pixel in original]

    new_array = []
    for pixel in new:
        new_array += [pixel.blue, pixel.green, pixel.red]

    new_red = [pixel.red for pixel in new]
    new_green = [pixel.green for pixel in new]
    new_blue = [pixel.blue for pixel in new]

    mserr = mse(original_array, new_array)
    mserr_red = mse(original_red, new_red)
    mserr_green = mse(original_green, new_green)
    mserr_blue = mse(original_blue, new_blue)
    snratio = snr(original_array, mserr)
    return mserr, mserr_red, mserr_green, mserr_blue, snratio


def main():
    if len(argv) >= 4:
        if argv[1] == "-e":
            with open(argv[2], "rb") as f:
                tga = f.read()
                header = tga[:18]
                footer = tga[len(tga) - 26:]
                width = tga[13] * 256 + tga[12]
                height = tga[15] * 256 + tga[14]
                tga = tga[18:]

            if width * height * 3 != len(tga):
                tga = tga[:-26]
            bitmap = PixelBitmap(tga, width, height)

            if len(argv) == 5:
                k = int(argv[4])
                print(k)
            else:
                k = 2

            low, high = encode(bitmap, k)
            with open(argv[3], "wb") as f:
                b = bytes(header)
                f.write(b + low + high)
            
        elif argv[1] == "-d":
            with open(argv[2], "rb") as f:
                coded = f.read()
                width = coded[13] * 256 + coded[12]
                height = coded[15] * 256 + coded[14]
                header = coded[:18]
                coded = coded[18:]

                length = int(len(coded) / 2)
                low = coded[:length]
                high = coded[length:]

            result = decode(low, high, width, height)

            with open(argv[3], "wb") as f:
                f.write(header + result)
            
        elif argv[1] == "-c":
            with open(argv[2], "rb") as f:
                original = f.read()
                header = original[:18]
                footer = original[len(original) - 26:]
                width = original[13] * 256 + original[12]
                height = original[15] * 256 + original[14]
                original = original[18:]

            if width * height * 3 != len(original):
                original = original[:-26]
            bitmap_original = PixelBitmap(original, width, height)
            bitmap_original = [
                bitmap_original[x, y]
                for y in reversed(range(bitmap_original.height))
                for x in range(bitmap_original.width)
            ]

            with open(argv[3], "rb") as f:
                new = f.read()
                header = new[:18]
                footer = new[len(new) - 26:]
                width = new[13] * 256 + new[12]
                height = new[15] * 256 + new[14]
                new = new[18:]

            if width * height * 3 != len(new):
                new = new[:-26]
            bitmap_new = PixelBitmap(new, width, height)
            bitmap_new = [
                bitmap_new[x, y]
                for y in reversed(range(bitmap_new.height))
                for x in range(bitmap_new.width)
            ]

            mse, mse_r, mse_g, mse_b, snr = tests(bitmap_original, bitmap_new)

            print("MSE:", mse)
            print("MSE (blue):", mse_b)
            print("MSE (green):", mse_g)
            print("MSE (red):", mse_r)
            print("SNR:", snr)
        
    else:
        print("BŁĄD: Niepopawne wywołanie programu.")
        print("main.py <-e/-d/-c> <input_file> [output_file] [k - default 2]")


if __name__ == "__main__":
    main()
