# Worley Noise Generator

## Intro

This project is an SDL2 based implementation of the Worley noise algorithm. 
The algorithm is simple, but creates some really interesting patterns,
and offers plenty of opportunities for customization to create your own 
designs. **This code can generate rapid flashing lights if you tell
it to.** If that is something that should be avoided,
I have some lovely 5 fps examples below that may offer
a safer alternative.

https://en.wikipedia.org/wiki/Worley_noise


## Dependencies

The project uses <a href="https://www.libsdl.org/">SDL2</a>
to handle the rendering.

## Usage

**Command line arguments**
|Argument|Description|
|---        |---|
|-points X  |Generate X central points for Worley noise algorithm|
|-w X       |Set window width to X|
|-h X       |Set window height to X|
|-d X       |Set image depth (Z-axis) to X|
|-seed X    |Use X as the seed for random point placement|
|-threads X |Number of threads to use for Worley noise algorithm|
|-s X       |Set speed of central points in pixels/frame|
|-invert    |Invert colors displayed|
|-colorcycle|Rotate intensity of RGB values|

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

To parse command line arguments, I am using a snippet
<a href="https://stackoverflow.com/users/85381/iain">iain</a>
provided on stackoverflow that
does a nice job.
https://stackoverflow.com/questions/865668/how-to-parse-command-line-arguments-in-c
## Examples

These recordings aren't really the best for displaying the output 
, but I've put together a couple examples
from different configurations I've tried. You should get a much cleaner result if you run the code natively.

**d<sub>1</sub> Worley noise generation with and without a
 specified depth.**

![](img/worleyDepth2.apng)
![](img/worley.apng)

**Multi-threaded runs: two threads vs four**

![](img/worley2thread.apng)
![](img/worley4thread.apng)

**Output with inverted colors and mixture of distance settings**

![](img/worleyInverted.apng)