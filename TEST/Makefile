# Define the total number of iterations for the loading bar
TOTAL_ITERATIONS := 50

# The main target that triggers the loading bar
all:
	@echo "Processing..."
	@i=0; while [ $$i -le $(TOTAL_ITERATIONS) ]; do \
		printf "["; \
		j=0; \
		while [ $$j -lt $$i ]; do \
			printf "="; \
			j=$$((j+1)); \
		done; \
		printf "] %d%%\r" $$((($$i * 100 + $(TOTAL_ITERATIONS) / 2) / $(TOTAL_ITERATIONS))); \
		sleep 0.1; \
		i=$$((i+1)); \
	done
	@echo "\nDone!"

.PHONY: all
