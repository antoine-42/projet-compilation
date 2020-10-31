# Projet Compilation

## Installation

- Installer GCC 9
- Installer openmpi
- En cas d'erreur, verifier que les flags de compilation pour openmpi sont corrects avec `mpicc --showme` / `mpicxx --showme`

## Lancer le code

```shell script
make clean && make test
```
