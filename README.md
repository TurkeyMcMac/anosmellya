# Anosmellya

Anosmellya is an evolution simulator.
The name is pronounced like "a nose smell ya," and is based on the word
"animalia" and the idea of smells.
As the name suggests, animals act based on the intensity of smells around them.
Unlike my other evolution simulators, this one has sexual reproduction.
The idea and parts of the code are borrowed from my project Intergrid.

## Compilation

### For Unix

Run `make`.

### For Windows (with MinGW)

Run this:

```
CXX="$ARCH-g++" PATH="$SDL/$ARCH/bin:$PATH" CXXFLAGS="-I$SDL/$ARCH/include -L$SDL/$ARCH/lib -static" make target=anosmellya.exe libs='`sdl2-config --static-libs`'
```

Where the variable `SDL` is set to the path of the directory you unzipped from
[here](http://libsdl.org/release/SDL2-devel-2.0.12-mingw.tar.gz) and `ARCH` is
either `x86_64-w64-mingw32` or `i686-w64-mingw32`.

A large (statically linked) executable `anosmellya.exe` will be produced.

## Usage

To run the program with a good configuration, run `./anosmellya` after
making the executable.

You can get a full list of options by running `./anosmellya -help`.

### Controls

The keyboard supplies some control at runtime.

* Press A to toggle visual smell affinity vectors, provided drawing was enabled.
This may slow things down.
A color coded vector is shown around an object scaled relative to the others.
Green is plant, blue is herbivore smell, red is carnivore smell, pink is baby
smell, and white is velocity.
* Press D to toggle drawing, provided it was enabled in the first place.
* Press F to go forward one simulation tick while paused.
* Press Q to quit.
* Press R to pause/unpause the simulation.
* Press S to toggle statistic printing.
* Press W to toggle waiting.
If it is off, then the simulation will run as fast as the computer can process.

### Configuration

Configuration is given in a simple `KEY = VALUE` format.
You can see the full list of configurable parameters, with explanations, in the
file `configurations/default.conf`.
Other interesting configurations are put in the same directory.
If a configuration lists working seeds, those seeds are guaranteed to produce
the same result on your computer only if it does single-precision floating-point
arithmetic the same as the computer on which each seed was tested.
Use the option `-conf FILE` to pass another configuration file.
Use the option `-cs STRING` to pass configuration as a string.
The format is the same as for a configuration file.

### Statistics

The `-print-stats` option enables statistics to be printed.
Each tick, a JSON object is printed on a new line of standard output.
The fields are as follows:

* `world_width`, `world_height`:
The width and height of the world in tiles.
* `herb_avg`:
The average stats of an herbivore.
* `herb_count`:
The herbivore population.
* `carn_avg`:
The average stats of a carnivore.
* `carn_count`:
The carnivore population.
* `plant_total`, `carn_total`, `herb_total`, `baby_total`:
The total of each smell across all tiles.

The `herb_avg` and `carn_avg` objects have the following keys:

* `age`:
The current age, in ticks.
* `baby_smell_amount`:
The baby smell produced per tick when fertile.
This may be negative.
* `baby_threshold`:
The amount of food above which the animal becomes fertile.
* `baby_food`:
The amount of food given by a mother to a newborn.
* `plant_aff`,  `herb_aff`, `carn_aff`, `baby_aff`:
The affinities for the different smells.
* `vel_aff`:
The velocity "smell" affinity.

All the fields ending in `aff` have the following keys:

* `impulse`:
The impulse vector as a two-item list where the first item's x and the second y.
* `plant_effect`, `herb_effect`, `carn_effect`, `baby_effect`:
The impulse effects for each smell.
* `food_effect`:
The impulse effect for the amount of food the animal has.

For more information about the meanings of the keys, see the
**Evolvable Traits** section.

## About the Simulation

Red carnivores eat herbivores.
Blue herbivores eat plants.
Green plants spontaneously appear, and are not alive.
The living animals act based on the smells around them.

### Smells

There are four smells in the simulation.
They all disperse and evaporate at configurable rates.
The smells are as follows:

* Plants:
Plants themselves are treated as a smell.
Plants are dropped in globs and are eaten by herbivores.
Plants do not grow, but rather evaporate like all other smells.
* Herbivore smell:
Herbivores produce a fixed amount of smell every tick.
This lets herbivores find kin and carnivores find prey.
* Carnivore smell:
Carnivores also produce smell for a similar purpose.
* Baby smell:
This is produced by an animal when it has enough food for a baby.
The intention is that others of the species use baby smell to find mates.
The amount produced per tick can evolve.
Note that one baby smell is shared between herbivores and carnivores.

### Evolvable traits

You can see the full list of traits that evolve by looking at `Animal::mutate`
in `src/Animal.cpp`.
The main evolving traits are the "smell affinities."
A smell affinity has several parts.
The main part is the "impulse," a 2D vector quantity.
An animal looks at the four adjacent squares and determines a smell gradient.
The impulse is rotated so that the smell gradient vector becomes the x-axis.
The impulse is changed based on several coefficients.
The coefficients correspond to all the smells on the current tile as well as the
animal's amount of food.
The coefficients are multiplied by their respective quantities then added to the
magnitude of the impulse.
The acceleration for a tick is the sum of all scaled impulses, normalized to a
configurable amount.
There are smell affinities for each of the smells.
There is also one where the input is the animal's velocity vector rather than a
smell gradient vector.
