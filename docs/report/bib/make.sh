rm all.bib
bibtool -s -d -- check.double.delete=on -- preserve.key.case=on *.bib > all.bib
emacs --batch all.bib -l untabify.el -f save-buffer

