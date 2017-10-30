# Probabilistic Robotics 2017/18

Maintainers:
 - [Giorgio Grisetti](https://sites.google.com/dis.uniroma1.it/grisetti)
 - [Bartolomeo Della Corte](http://www.dis.uniroma1.it/~dellacorte/)
 - [Dominik Schlegel](https://sites.google.com/dis.uniroma1.it/schlegel)

Contents: <br/>
Teaching material for the [Probabilistic Robotics course](https://sites.google.com/dis.uniroma1.it/probabilistic-robotics) of the academic year 2017/2018

Supported environments: <br/>
Ubuntu 14.04 LTS, Ubuntu 16.04 LTS

## What's where?
| Folder       | Description                                                                             |
| :----------- | :-------------------------------------------------------------------------------------- |
| literature   | Additional reading material related to the coursework                                   |
| applications | Octave/C++ example programs discussed in the course                                     |
| slides       | Lecture slides in PDF format                                                            |

## Want to have all the files on your computer?

Install [git](https://git-scm.com/):

    sudo apt-get install git
  
Clone (download) this repository somewhere on your computer:

    git clone https://gitlab.com/grisetti/probabilistic_robotics_2017_18
  
Later, as new material is added - one can conveniently update the local copy:

    cd probabilistic_robotics_2017_18
    git pull

## Application guide
### Requirements
Install [GNU Octave](https://www.gnu.org/software/octave/) or [MATLAB](https://mathworks.com/products/matlab.html) on your machine. <br/>

`Octave` can be installed by simply entering:

    sudo apt-get install octave
    
If you want to run the `Octave` examples on a Windows machine make sure to copy all files from
[visualization](https://gitlab.com/grisetti/probabilistic_robotics_2017_18/tree/master/applications/octave/tools/visualization) and
[utilities](https://gitlab.com/grisetti/probabilistic_robotics_2017_18/tree/master/applications/octave/tools/utilities)

### Applications

 - [grid-orazio](https://gitlab.com/grisetti/probabilistic_robotics_2017_18/tree/master/applications/octave/04_grid_orazio) (Octave)
 - [ekf_localization](https://gitlab.com/grisetti/probabilistic_robotics_2017_18/tree/master/applications/octave/01_ekf_localization) (Octave)
 - [ekf_localization_bearing_only](https://gitlab.com/grisetti/probabilistic_robotics_2017_18/tree/master/applications/octave/02_ekf_localization_bearing_only) (Octave)
 - [ekf_slam](https://gitlab.com/grisetti/probabilistic_robotics_2017_18/tree/master/applications/octave/10_ekf_slam) (Octave)
 - [ukf_localization](https://gitlab.com/grisetti/probabilistic_robotics_2017_18/tree/master/applications/octave/14_ukf_localization) (Octave)

## Robotic news
[IEEE spectrum](https://spectrum.ieee.org/robotics) <br/>
[arXiv public library](https://arxiv.org/list/cs.RO/recent) <br/>
