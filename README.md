MapGenerator
============

Generation of procedural maps. The main idea behind the algorithm is taken from [Amit's Polygonal Map Generation for Games](http://www-cs-students.stanford.edu/~amitp/game-programming/polygon-map-generation/).
It has the following steps:
* Place a number of random but uniformly distributed points (using Poisson Disk Sampling).
* Calculate the Voronoi diagran of the points.
* Using Perlin Noise determine which cells are land and which are water.
* Calculate the elevation of each point as its distance to the sea. Normalize all the heights.
* Place river sources in random spots and let them flow downwards.
* Calculate the moisture od each point given its distance to the sea (less moisture) and rivers (more moisture)

And that's pretty much it. You can find a couple of results [here](http://imgur.com/a/RXQi4). The repo also contains a Poisson Disk Sampling implementation and a Markov Chains based name generator.

