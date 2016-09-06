# huller

Simple cli util to find the convex hull of a transparent png and output it
as a lua table of x/y coordinates. Useful for making collision shapes for
for sprites.

Adapted from the opencv convex hull example:
http://docs.opencv.org/3.0-beta/doc/tutorials/imgproc/shapedescriptors/hull/hull.html


## Dependencies

* [OpenCV >= 3.1](http://opencv.org/downloads.html)
* make

## Compiling

```
make
```

## Using

```
./huller path/to/sprite.png > sprite.lua
```

`sprite.lua` will contain a lua module you can require and to have a table of
x/y coordinates of a convex hull shape for the sprite image.

This is useful for working with a collision library like
[HC](http://hc.readthedocs.io/en/latest/)
