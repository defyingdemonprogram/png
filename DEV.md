## PNG Specification

### PNG Signature

Every PNG file begins with the same fixed 8-byte signature, represented by the following decimal values:

`137 80 78 71 13 10 26 10`

This signature identifies the file as a PNG and ensures that the following data is a valid PNG image. The image data is organized into a series of chunks, starting with an `IHDR` chunk and ending with an `IEND` chunk.

### PNG Chunk Naming Convention

In the PNG file format, **chunk types** are identified by **four ASCII letters**, with each letter's **case (uppercase/lowercase)** encoding specific **properties** using **bit 5 (value 32)** of each byte. This design allows decoders and editors to interpret unknown chunks more intelligently.

Each of the 4 characters in the chunk name has a specific bit that conveys the following:

1. **Ancillary bit (1st letter)**

   * **Uppercase (0)** = **Critical** (essential to image decoding)
   * **Lowercase (1)** = **Ancillary** (optional for decoding)

2. **Private bit (2nd letter)**

   * **Uppercase (0)** = **Public** (standard or registered)
   * **Lowercase (1)** = **Private** (application-specific)

3. **Reserved bit (3rd letter)**

   * **Must be Uppercase (0)** in current PNG version
   * **Lowercase (1)** = **Invalid in current standard**

4. **Safe-to-copy bit (4th letter)**

   * **Uppercase (0)** = **Unsafe to copy** if unrecognized
   * **Lowercase (1)** = **Safe to copy**

For example, chunk name **cHNk**:

* `c` = ancillary (1)
* `H` = public (0)
* `N` = reserved bit = 0 (valid)
* `k` = safe to copy (1)
  So, **cHNk** is an *ancillary, public, valid, and safe-to-copy* chunk.


### Endian Issue in PNG

* PNG uses **big-endian** byte order for encoding integer values. However, most modern architectures such as ARM and x86 use **little-endian** byte order, where the least significant byte is stored first. In contrast, **big-endian** stores the most significant byte first.

  For example, the hexadecimal number `0x12345678` would be stored in memory as:

  * **Big-endian:** `12 34 56 78`
  * **Little-endian:** `78 56 34 12`

  Examples of big-endian architectures include older Motorola processors (e.g., the 68000 series) and some network protocols that follow network byte order (which is big-endian).

* Since PNG files require data to be written in big-endian format, and many systems natively use little-endian, we need to **reverse the byte order** of multi-byte integers when writing to (or reading from) a PNG file on a little-endian machine. This ensures proper interpretation and compatibility with the PNG specification.



### References:

* [PNG Signature - PNG Datastream Structure](https://www.w3.org/TR/2003/REC-PNG-20031110/#5PNG-file-signature)
