## PNG Specification

### PNG Signature

Every PNG file begins with the same fixed 8-byte signature, represented by the following decimal values:

`137 80 78 71 13 10 26 10`

This signature identifies the file as a PNG and ensures that the following data is a valid PNG image. The image data is organized into a series of chunks, starting with an `IHDR` chunk and ending with an `IEND` chunk.

### References:

* [PNG Signature - PNG Datastream Structure](https://www.w3.org/TR/2003/REC-PNG-20031110/#5PNG-file-signature)
