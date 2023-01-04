# generate-test-images
Takes a sample image of HEIC format and rapidly generates a large number of derivative binary distinct versions that have a small number of bytes modified such that they have unique hashes. There is no attempt to understand the HEIC format, this is more shotgun rather than scalpel.  The resulting images /should/ be valid HEIC but visually meaningless (potentially visually corrupt). 

Intended to generate a large number of files VERY rapidly (24 secs for 10,000 on an m1 mac).  The images are in a sense corrupted (visually to a greater or esser extent) but should parse as valid.  
Different runs of the program should generate binary distinct output.  

There is NO licence or warranty with this program.  Use at your own risk for your own purpose(s).  If you do find it useful
I'd appreciate a courtesy mention in your code.  

It's a self contained ansi C file that should build with any reasonable C compiler.  No exotic language features are used and no external dependencies.  It has been tested /only/ with Apple clang version 14.0.0 (clang-1400.0.29.202) on m1 mac.  
