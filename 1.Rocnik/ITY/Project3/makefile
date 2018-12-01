NAME=proj3

all: clean do

do: $(NAME).tex
	latex $(NAME).tex
	bibtex $(NAME) || true
	latex $(NAME).tex
	latex $(NAME).tex
	dvips -t a4 $(NAME).dvi
	ps2pdf -sPAPERSIZE=a4 $(NAME).ps

clean:
	rm -f $(NAME).dvi $(NAME).log $(NAME).aux $(NAME).ps $(NAME).pdf
