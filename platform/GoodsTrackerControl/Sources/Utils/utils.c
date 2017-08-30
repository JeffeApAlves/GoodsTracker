/*
 * utils.c
 *
 *  Created on: 07/06/2017
 *      Author: Jefferson
 */

#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include "Array.h"
#include "utils.h"
#include "protocol.h"

void str_split(List* result, char* str, const char a_delim) {

	char delim[2];
	delim[0]	= a_delim;
	delim[1]	= 0;
    char*	tmp	= str;
    int	count	= 0;

	while (*tmp){

		if (a_delim == *tmp){
			count++;
		}

		tmp++;
	}

	result->count	= count + 1;
	result->itens	= _malloc(sizeof(char*) * result->count);

	unsigned int len = strlen(str);

	buffer_t token, state;

	token = memtok(str, len, delim, &state);

    unsigned short idx = 0;

	while (token.text != NULL) {

		char* tmp = _malloc(sizeof(char) * (token.len + 1));

	   if(tmp!=NULL){

		   memcpy(tmp, token.text, token.len);

		   tmp[token.len] = '\0';

		   result->itens[idx++] = tmp;

		   token = memtok(NULL, 0, delim, &state);
	   }
	}
}
//------------------------------------------------------------------------

void getField(char* str, char* out,uint16 num,const char a_delim) {

	uint16 i,start,end,f;

	f		= 0;
	start	= 0;
	end		= 0;
	out[0]	= '\0';

	for(i=0;i<ARRAY_LEN_PAYLOAD;i++){

		if(str[i]=='\0'){

			end = i;
			break;
		}

		if(str[i]==a_delim){

			f++;

			if(f==num){
				start = i+1;
			}

			if(f==(num+1)){

				end = i;
				break;
			}
		}
	}

	if((end-start)>0){

		strncpy(out,str+start,end-start);
		out[end-start] = '\0';
	}
}
//------------------------------------------------------------------------

int getNumField(char* str,const char a_delim) {

	uint16 i,f;

	i		= 0;
	f		= 0;

	while((str[i]!='\0') && (i<ARRAY_LEN_PAYLOAD)){

		if(str[i++]==a_delim){

			f++;
		}
	}

	return f+1;
}
//------------------------------------------------------------------------

/*
 *
 *  Use strpbrk() for multiple delimiters.
 *
 **/
buffer_t memtok(const void *s, size_t length, const char *delim, buffer_t *save_ptr){

    const unsigned char *stream,
                        *token;
    size_t len = 0;

    if (NULL == s) {
        stream = save_ptr->text;
    } else {
        stream = s;
        save_ptr->len = length;
    }

    token = stream;

    /* Advance until either a token is found or the stream exhausted. */
    while (save_ptr->len--) {
        if (memchr(delim, *stream, strlen(delim))) {
            /* Point save_ptr past the (non-existent) token. */
            save_ptr->text = stream + 1;
            return (buffer_t) { .text = token, .len = len };
        }

        ++len;
        ++stream;
    }

    /* State : done. */
    *save_ptr = (buffer_t) { .text = NULL, .len = 0 };

    /* Stream exhausted but no delimiters terminate it. */
    return (buffer_t){ .text = token, .len = len };
}
//-----------------------------------------------------------------------------------------

void str_append(char subject[], char insert[], int pos) {

	size_t size = (strlen(subject)-pos) + (strlen(insert) + 1);

	if(size>0){

		char* buf	= (char*) _malloc(sizeof(char)*size);

		if(buf!=NULL){

			memset((void*)buf,0,size);

			strcpy(buf, insert);
			strcpy(buf+strlen(insert), subject + pos);
			strcpy(subject + pos , buf);

			_free(buf);
		}
	}
}
//------------------------------------------------------------------------

void removeList(List* list){

	int i;

	// Exlcui cada item
	for(i=0 ; i<list->count ; i++){

		_free(list->itens[i]);
	}

	// Exclui o array dos itens
	_free(list->itens);
}
//------------------------------------------------------------------------

void AsInteger(int* out,char *str,uint16 index,const char a_delim){

	char field[10];

	getField(str,field,index,a_delim);

	*out = atoi(field);
}
//------------------------------------------------------------------------

void AsString(char* out,char *str,uint16 index,const char a_delim){

	getField(str,out,index,a_delim);
}
//-----------------------------------------------------------------------

void AsHex(int* out,char *str,uint16 index,const char a_delim){

	char field[10];

	getField(str,field,index,a_delim);

	*out = strtol(field, NULL, 16);
}
//-----------------------------------------------------------------------

void AsFloat(float* out,char *str,uint16 index,const char a_delim){

	char field[20];

	getField(str,field,index,a_delim);

	*out = atof(field);
}
//------------------------------------------------------------------------

void AsChar(char* out,char *str,uint16 index,const char a_delim){

	char field[2];

	getField(str,field,index,a_delim);

	*out = field[0];
}
//------------------------------------------------------------------------

void AsResource(Resource* out,char *str,uint16 index,const char a_delim){

	char field[4];

	getField(str,field,index,a_delim);

	*out = getResource(field);
}
//------------------------------------------------------------------------
