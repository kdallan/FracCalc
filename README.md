# FracCalc
Fraction Calculator

This project is built for OSX.

Open the FracCalc.xcodeproj in XCode. Then build.

Once built. Go under products in the left-most pane and you should see FracCalc. Right click and choose Show In Finder to take you to its location. Then, with terminal, navigate to this location and type ./FracCalc to run.

Alternatively you can run the command line directly in XCode itself. Just run it with the play button.

The Calculator has two modes. If you pass in any parameters FracCalc will combine them and evaluate the expression.
As an example:

    ./FracCalc 3/4 * -1/2
    
Will evaluate the expression 3/4 * -1/2 when you press return. You should get:

    = -3/8
    
The calculator will finish running.

If you don't supply any parameters you will go into calculator mode and you should see:

  Fraction Calculator for OneLogin
  Type in your fraction expression then press return
  You can type 'quit' to exit the calculator
  ?
  
To exit this mode and return to the command line prompt, you must type quit.

The calculator can perform + - / and * operations. It also works with opening and closing braces. * and + operators have higher precidence than + and - and will be performed first.

To input a fraction with a whole you must use a _ separator. As an example:

  3_1/2
  
If you compile the code with ALLOW_WHOLE_FRAC_SPACE defined, FracCalc will allow a space between the whole and fractional part as well as using _. So the following will be identical

  3 1/2
  3_1/2
  
If you compile the code with RUN_UNIT_TESTS defined, the set of unit tests will run instead of the interactive calculator.
















