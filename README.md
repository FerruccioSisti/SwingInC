# SwingInC
Repo for translation tasks given by Dr. Chen for TA'ing COMP-2800

Dr. Chen has asked me to translate her assignments given out using Java swing to C using gtk.

Make sure you download the gtk library. This can be done by running <b>brew install gtk</b> <br> in your terminal. I used zsh. <br>
Files are compiled by using: <br>
<b>gcc \`pkg-config --cflags gtk+-3.0\` -o executableName filename.c \`pkg-config --libs gtk+-3.0\`</b>
