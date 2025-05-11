# PNG

PNG (Portable Network Graphics) is a file format designed for the lossless, portable, and efficient storage of raster images. It serves as a patent-free replacement for GIF and can substitute for many common uses of TIFF.

### Quick Start

1. **Encrypt a message into a PNG file**
   Provide an input image (PNG format) and specify the output image that will contain the hidden message.
   Run the following commands to build and execute the encryption:

   ```bash
   ./build.sh
   ./png sample_transparent_image.png output.png
   ```

2. **Decode the hidden message from the PNG file**
   To extract the encrypted message from the output image, run:

   ```bash
   ./decode output.png
   ```

### References

* [Portable Network Graphics (PNG) Specification – W3C](https://www.w3.org/TR/2003/REC-PNG-20031110/)
* [PNG Specification – libpng](http://www.libpng.org/pub/png/spec/)
