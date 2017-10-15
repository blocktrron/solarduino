all:

docs:
	@cd $(CURDIR)/docs && doxygen Doxyfile

clean:
	@rm -rf $(CURDIR)/docs/html
	@rm -rf $(CURDIR)/docs/latex

.PHONY : clean docs