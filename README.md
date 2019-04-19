*Why are the electromagnetic waves slowed down when they travel through medium? Having looked through hoards of articles on this topic, barely did I find an answer to my question.* 

##ABSTRACT:
The electromagnetic radiation propagate in every medium at the speed of light in vacuum. However, the measurements will show you the opposite: phase of the wave falling on a dielectric plate will appear to be slightly shifted back from the expected value after propagating through the plate. The wave knows nothing about the medium with which it interacts, so how can it ever be possible? The thing is that secondary electromagnetic waves produced by the medium's electrons oscillate due to the source (primary) electromagnetic wave and thereafter, interfering with it, make it look like as if the primary wave has been slowed down (in the dielectrics) or reflected fully (in the superconductors). 

The nature of this effect has been well explained by Feynman in the **"Lectures on Physics", vol.1, chapter 31:**
> It is approximately true that light or any electrical wave does appear to travel at the speed c/n through a material whose index of refraction is n, but the fields are still produced by the motions of all the charges—including the charges moving in the material—and with these basic contributions of the field travelling at the ultimate velocity c. Our problem is to understand how the apparently slower velocity comes about.

> Why should there be charges moving in the glass? We know that all material consists of atoms which contain electrons. When the electric field of the source acts on these atoms it drives the electrons up and down, because it exerts a force on the electrons. And moving electrons generate a field—they constitute new radiators. These new radiators are related to the source, because they are driven by the field of the source. The total field is not just the field of the source, but it is modified by the additional contribution from the other moving charges. This means that the field is not the same as the one which was there before the glass was there, but is modified, and it turns out that it is modified in such a way that the field inside the glass appears to be moving at a different speed. That is the idea which we would like to work out quantitatively.

> Now this is, in the exact case, pretty complicated, because although we have said that all the other moving charges are driven by the source field, that is not quite true. If we think of a particular charge, it feels not only the source, but like anything else in the world, it feels all of the charges that are moving. It feels, in particular, the charges that are moving somewhere else in the glass. So the total field which is acting on a particular charge is a combination of the fields from the other charges, whose motions depend on what this particular charge is doing!

Having hardly tried applying mathematics, I admittedly didn't manage to understand why does the interaction between the medium's electrons and the source wave give this very result - the source wave appearing to be slowed down. So then I decided to write a script. Let the computer do all the maths involved in the process.

##REQIUREMENTS:
- OS: Ubuntu\Debian

##HOW TO LAUNCH:
- compile the script.c manually:
	1. install glut:
		apt-get install libgl1-mesa-dev freeglut3-dev libglew-dev
	2. compile the script.c with gcc:
		gcc /path/to/file/script.c -lGL -lGLU -lGLEW -lglut -lm -o /path/to/file/script
	3. launch the executable file:
		/path/to/file/script
- or simply download the compiled file called "script" and launch it:
	1. /path/to/file/script
