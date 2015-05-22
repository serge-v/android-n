set -e

export PATH=/usr/local/bin:~/bin:$PATH

convert -background black -fill DodgerBlue -size 160x160 -colors 256 -depth 8 -draw 'circle 80,80 135,135' -fill white label:O oxygen.png
mkresource oxygen.png oxygen_png oxygen.png.h oxygen.png.c

convert -background white -fill black -size 160x160 -colors 256 -depth 8 -draw 'circle 80,80 135,135' label:O oxygen_mask.png
mkresource oxygen_mask.png oxygen_mask_png oxygen_mask.png.h oxygen_mask.png.c
