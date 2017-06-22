/*
 * utils.c
 *
 *  Created on: 07/06/2017
 *      Author: Jefferson
 */

#include <string.h>
#include <assert.h>
#include "utils.h"

void str_split(List* result,char* str, const char a_delim){

    char delim[2];
    delim[0]			= a_delim;
    delim[1]			= 0;

    char*	tmp			= str;
    char*	sptr		= str;
    result->size		= 0;
    char*	last_comma	= NULL;
    result->itens		= NULL;
    char*	token		= NULL;

    if(str!=NULL){

		while (*tmp){

			if (a_delim == *tmp){

				result->size++;
	            last_comma = tmp;
			}

			tmp++;
		}

		result->size += last_comma < (str + strlen(str) - 1);

		if(result->size>0){

			result->itens = _malloc(sizeof(char*) * result->size);

			if (result->itens) {

				size_t idx = 0;

				*(result->itens + idx) = NULL;

				Itens itens;

				int n = replace(str,delim[0], '\0',itens);

				int i=0;

				for(i;i<n;i++){

					tmp = _malloc((strlen(itens[i]) + 1)*sizeof(char));

					memset(tmp,0,strlen(itens[i]) + 1);

					strcpy(tmp, itens[i]);

					*(result->itens + idx++) = tmp;
				}
/*
				while ((token = strtok_r(sptr, delim, &sptr)) != NULL) {

					assert(idx < result->size);

					tmp = _malloc((strlen(token) + 1)*sizeof(char));

					memset(tmp,0,strlen(token) + 1);

					strcpy(tmp, token);

					*(result->itens + idx++) = tmp;
				}*/
			}
		}
    }
}
//------------------------------------------------------------------------

int replace(char* str,char c_old, char c_new,Itens itens){

	int n = 0;
	int i =0;
	size_t s = strlen(str)+1;

	while(str[i]!='\0'){

		if(str[i]==c_old){
			str[i] = c_new;
			n++;
		}
		i++;
	}

	itens = _malloc(sizeof(char*) * (n++));

	itens[0] = str;

	int j =1;

	for(i=1;i<s;i++){

		if(str[i]=='\0'){

			itens[j++] = (str+i+1);
		}
	}

	return n;
}
//------------------------------------------------------------------------

void str_append(char subject[], char insert[], int pos) {

	size_t size = (strlen(subject)-pos) + (strlen(insert) + 1);

	if(size>0){

		char* buf	= (char*) _malloc(sizeof(char)*size);

		if(buf!=NULL){

			memset(buf,0,size);

			strcpy(buf, insert);
			strcpy(buf+strlen(insert), subject + pos);
			strcpy(subject + pos , buf);

			_free(buf);
		}
	}
}
//------------------------------------------------------------------------

void removeList(List* list){

	int i = 0;

	for(i=0 ; i<list->size ; i++){

		_free(list->itens[i]);
	}

	_free(list->itens);
}
//------------------------------------------------------------------------
