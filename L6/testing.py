from main import PixelBitmap, encode, decode, tests

for k in range(1, 8):
    for i in range(4):
        inputimg = "testy/example" + str(i) + ".tga"
        outputimg = "output/kikd.kkd"
        resultimg = "results/" + str(i) + str(k) + ".tga"
        errortxt = "errors/" + str(i) + str(k) + ".txt"

        with open(inputimg, "rb") as f1:
            tga = f1.read()
            header = tga[:18]
            footer = tga[len(tga) - 26:]
            width = tga[13] * 256 + tga[12]
            height = tga[15] * 256 + tga[14]
            tga = tga[18:]

        if width * height * 3 != len(tga):
            tga = tga[:-26]
        bitmap = PixelBitmap(tga, width, height)

        low, high = encode(bitmap, k)
        with open(outputimg, "wb") as f2:
            b = bytes(header)
            f2.write(b + low + high)
        

        with open(outputimg, "rb") as f3:
            coded = f3.read()
            width = coded[13] * 256 + coded[12]
            height = coded[15] * 256 + coded[14]
            header = coded[:18]
            coded = coded[18:]

            length = int(len(coded) / 2)
            low = coded[:length]
            high = coded[length:]

        result = decode(low, high, width, height)

        with open(resultimg, "wb") as f4:
            f4.write(header + result)
        

        with open(inputimg, "rb") as f5:
            original = f5.read()
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

        with open(resultimg, "rb") as f6:
            new = f6.read()
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

        with open(errortxt, "w") as f7:
            f7.write("MSE: " + str(mse) + "\n")
            f7.write("MSE b: " + str(mse_b) + "\n")
            f7.write("MSE g: " + str(mse_g) + "\n")
            f7.write("MSE r: " + str(mse_r) + "\n")
            f7.write("SNR: " + str(snr) + "\n")
