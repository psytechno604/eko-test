# eko-delivery-test

Program written as Console C++ application in Visual Studio 2019 Preview.

Catch2 library (https://github.com/catchorg/Catch2) used for unit tests.

## Building
### Windows
Project created in Visual Studio 2019 Preview.
### Linux
Clone the repo, cd into **eko-delivery** folder, then
```console
cmake .
make
```
## Usage
### Delivery cost for route:

```console
<executable> [graph] [route]
```
e.g.
```console
<executable> AB1,AC4,AD10,BE3,CD4,CF2,DE1,EB3,EA2,FD1 A-B-E
```
(without spaces between graph paths)

### Number of routes:
```console
<executable> [graph] [from] [to] [useSameRouteTwice] [maxSteps] [maxCost]
```
e.g. 
```console
<executable> AB1,AC4,AD10,BE3,CD4,CF2,DE1,EB3,EA2,FD1 E D false 4 INT_MAX
```

### The cost of cheapest delivery route:
```console
<executable> [graph] [from] [to]
```
e.g. 
```console
<executable> AB1,AC4,AD10,BE3,CD4,CF2,DE1,EB3,EA2,FD1 E D
```

### To run pre-defined tests:
```console
<executable> --test 
```
