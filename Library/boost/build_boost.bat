@echo off

b2 stage ^
-a ^
--reconfigure ^
--toolset=msvc-14.0 ^
--variant=release ^
-s ZLIB_SOURCE=D:\NAMEHERE\Source\Library\zlib\include\zlib ^
-s ZLIB_BINARY=D:\NAMEHERE\Source\Library\zlib\lib\release ^
--prefix=D:\NAMEHERE\Source\Library\boost ^
--libdir=D:\NAMEHERE\Source\Library\boost\lib ^
--includedir=D:\NAMEHERE\Source\Library\boost\include

PAUSE