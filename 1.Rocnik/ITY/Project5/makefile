NAME=proj5

all: clean do

do: $(NAME).tex
	latex $(NAME).tex
	pdflatex $(NAME).tex
			
clean:
	rm -f $(NAME).dvi $(NAME).log $(NAME).aux $(NAME).ps $(NAME).pdf $(NAME).nav $(NAME).out $(NAME).toc $(NAME).snm
