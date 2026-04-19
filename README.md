# About
A program that allows you to draw digits that a neural network can recognise, trained on the MNIST database. 

At first, I used the MSE loss function + sigmoid for activation. This was useful in learning the math behind the backpropagation algorithm but led to poor prediction results.

To improve the accuracy of the model, I changed to the cross entropy loss function + softmax and relu for hidden layers, and also added the functionality for multiple epochs. This combination allowed me to easily implement the backpropagation algorithm as the derivative simplified to a - y.

# Build

Create a build folder
```
cd path/to/digit-identifier
mkdir build
```

Run cmake and make from inside the build folder
```
cd build
cmake ..
make
```

Run the program
```
./CR
```
```
```
