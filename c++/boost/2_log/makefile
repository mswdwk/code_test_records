Q=@
CXX=g++
CXXFLAGS=-g -std=c++11 -Wall -fprofile-arcs -ftest-coverage -lgcov \
 -DBOOST_LOG_DYN_LINK

LDFLAGS:=-lboost_log_setup -lboost_log -lpthread -lboost_thread 
CXXOBJS:=$(subst .cc,.o,$(wildcard *.cc))
CXXOBJS+=$(subst .cpp,.o,$(wildcard *.cpp))
BINS:=$(subst .cc,,$(wildcard *.cc))
BINS+=$(subst .cpp,,$(wildcard *.cpp))

.PHONY:all
all:$(BINS)
	$Qecho $(BINS)		
run:
	$Q success_cases=();failed_cases=(); \
		failed_count=0;success_count=0;\
		for b in $(BINS);\
		do \
		echo "start test: $$b";\
		./$$b; \
		let ret=$$?; \
		echo "end test: $$b,ret=$$ret";\
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

%.o:%.cc
	$(CXX) $(CXXFLAGS)  $< -o $@ $(LDFLAGS)

%.o:%.cpp
	$(CXX) $(CXXFLAGS)  $< -o $@ $(LDFLAGS)

clean:
	rm -rf *.o core* a.out $(BINS) *.gc* *.info cover_* *.log
