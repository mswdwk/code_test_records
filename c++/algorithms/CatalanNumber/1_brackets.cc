#include<iostream>
#include<string>
#include<set>

using namespace std;

void cout_a_bracket_with_printed_str(set<string> s){
	long id = 1;
	for(set<string>::iterator it = s.begin(); it != s.end(); ++it ){
		cout<<"id="<<id++<<"\t,"<<(*it)<<endl;
	}
}

// bracket_set is the n - 1, all output strings
// TODO: use reference: & bracket_set
void bracket_nset(int n,set<string> bracket_set,long & kind_count){
	if(n < 1 ){
		cout<<"invaild input: "<<n<<endl;
		return;
	}
	else if(1 == n ){
		// deal only 1 pair bracket , when bracket_set is empty.
		if ( bracket_set.size() < 1 ){
				cout<<"()"<<endl;
		}else{
			set<string> s;
			for(set<string>::iterator it = bracket_set.begin();
				it != 	bracket_set.end();++it) {
					s.insert("()"+*it);
					s.insert(*it+"()");
					s.insert("("+ *it+")");
			}
			kind_count = s.size();
			cout_a_bracket_with_printed_str(s);
		}
		return;
		// n > 1
	} else {
		set<string> new_set;
		if(bracket_set.size() < 1){
			new_set.insert("()");
		} else {
			for(set<string>::iterator it = bracket_set.begin();
				it != bracket_set.end(); ++it) {
				// cout_a_bracket_with_printed_str(*it);
				new_set.insert("()"+ *it);
				new_set.insert(*it+"()");
				new_set.insert("("+ *it+")");
			}
		}
		bracket_nset(n-1,new_set,kind_count);
	}
}

void bracket(int n){
	cout<<"hello bracket:"<<n<<endl;
	set<string> bracket_set;
	long kind_count = 1;
	bracket_nset(n,bracket_set,kind_count);
	cout<<"Answer: "<<n<<" pairs of bracket, can have "<<kind_count<<" kinds of legal combination"<<endl;
}

int main()
{
	bracket(1);
	bracket(2);
	bracket(3);
	bracket(4);
	bracket(5);
	cout<<"hello"<<endl;
}

