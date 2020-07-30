# Worley Noise Generator(WIP)

## Intro

This project is an SDL2 based implementation of the Worley noise algorithm. 
The algorithm is simple, but creates some really interesting patterns,
and offers plenty of opportunities for customization to create your own 
designs. **This code can generate rapid flashing lights if you tell
it to.** If that is something that should be avoided,
I have some lovely 5 fps example gifs below that may offer
a safer alternative.

https://en.wikipedia.org/wiki/Worley_noise


## Dependencies

The project uses <a href="https://www.libsdl.org/">SDL2</a>
to handle the rendering.

## Usage

## References

I was first introduced to Worley noise by The Coding Train's
video on the topic where he does a great job on explaining it. I've
placed a link below for both his video, and his code examples hosted
on Github.

<a href="https://www.youtube.com/watch?v=4066MndcyCk">Coding in the Cabana 4: Worley Noise</a>

https://github.com/CodingTrain/WorleyNoise

Most of my knowledge for working with SDL comes Lazy Foo's tutorial series where
they have dedicated a lot of time and effort to creating some great lessons.

<a href="https://lazyfoo.net/tutorials/SDL/index.php">Lazy Foo' Productions</a>

To parse commandline arguments, I am using a snippet
<a href="https://stackoverflow.com/users/85381/iain">iain</a>
provided on stackoverflow that
does a nice job.
https://stackoverflow.com/questions/865668/how-to-parse-command-line-arguments-in-c
## Examples

GIFs aren't really the best for displaying the output 
(my setup isn't great either), but I've put together a couple examples
from different configurations I've tried. You should get a much cleaner result if you run the code natively.

**d<sub>1</sub> Worley noise generation with and without a
 specified depth.**

![](img/worleyDepth2.apng)
![](img/worley.apng)

**Multithreaded runs: two threads of execution vs four**

![](img/worley2thread.apng)
![](img/worley4thread.apng)

**Output with inverted colors and mixture of distance settings**

![](img/worleyInverted.apng)