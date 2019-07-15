filename = proj4
make:
	latex ${filename}.tex
	bibtex ${filename}.aux
	latex ${filename}.tex
	latex ${filename}.tex
	dvips ${filename}.dvi
	ps2pdf ${filename}.ps
