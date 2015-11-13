set -e

export PATH=/usr/local/bin:~/bin:$PATH

convert -background black -fill DodgerBlue -size 256x256 -colors 256 -depth 8 -draw 'circle 80,80 135,135' label:O oxygen.png
mkresource oxygen.png oxygen_png oxygen.png.h oxygen.png.c

convert -background white -fill black -size 256x256 -colors 256 -depth 8 -draw 'circle 80,80 135,135' label:O oxygen_mask.png
mkresource oxygen_mask.png oxygen_mask_png oxygen_mask.png.h oxygen_mask.png.c

convert -background black -fill gray27 -size 256x256 -colors 256 -depth 8 -draw 'circle 80,80 135,135' label:O carbon.png
mkresource carbon.png carbon_png carbon.png.h carbon.png.c

convert -background white -fill black -size 256x256 -colors 256 -depth 8 -draw 'circle 80,80 135,135' label:O carbon_mask.png
mkresource carbon_mask.png carbon_mask_png carbon_mask.png.h carbon_mask.png.c
