Q=@
GOFLAGS=-ldflags "-s"
TARGETS:=$(subst .go,,$(wildcard *.go))
.PHONY: all clean

all:${TARGETS}
	@echo "TARGETS: ${TARGETS}"

%:%.go
	go build ${GOFLAGS} -o $@ $<

run:
	$Q success_cases=();failed_cases=(); \
                failed_count=0;success_count=0;\
                for b in $(TARGETS);\
                do \
                echo "[`date`] start test: $$b";\
                ./$$b; \
                let ret=$$?; \
                echo "[`date`] end test: $$b,ret=$$ret";\
                if [ $${ret} -ne 0 ];then \
                        failed_cases[$$success_count]=$$b;\
                        let failed_count+=1;\
                else \
                        success_cases[$$success_count]=$$b;\
                        let success_count+=1;\
                fi \
        done;\
        echo "success_count=$$success_count";\
        echo "success_cases=$${success_cases[*]}"; \
        echo "failed_count=$$failed_count"; \
        echo "failed_cases=$${failed_cases[*]}"; \

clean:
	rm ${TARGETS} 
