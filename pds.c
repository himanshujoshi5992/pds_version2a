#include "pds.h"
#include <stdio.h>
#include <stdlib.h>
#include "contact.h"

struct PdsInfo pdsInfo;
FILE *repo_fptr = NULL;
FILE *ndx_fptr = NULL;
struct PdsNdxInfo pdsNdxInfo;
int count = 0,number = 0;
int index_array[100];


int pds_store( struct Contact *c1 ){

	struct Contact c2 = *c1;
	
	int status = pds_search_by_key(c2.contact_id,&c2);
	
	if(repo_fptr && (status == PDS_REC_NOT_FOUND)){
		fwrite(c1,sizeof(struct Contact),1,repo_fptr);
		
		pdsNdxInfo.offset = (116*(number++));
		index_array[count++] = c1->contact_id;
		index_array[count++] = pdsNdxInfo.offset;
		printf("%d:%d\n", index_array[count - 2],index_array[count - 1]);
		if(!fwrite(index_array,sizeof(int),sizeof(int)*2,ndx_fptr))
		{
			printf("Could not store the index_array!\n");
		}
		
		return PDS_SUCCESS;
	}
	else {
		return PDS_DUPLICATE_RECORD;
	}

	return PDS_STORE_ERROR;
}

int pds_search_by_key( int key, struct Contact *c ){

	int offset,f_key;
/*
    fseek(repo_fptr, 0L, SEEK_SET);
    if(pdsInfo.repo_status == 1){
    	while(fread(c, sizeof(struct Contact), 1, repo_fptr)){
	    	if(c->contact_id == key){
	    		//printf("Contact_id= %d\n", c->contact_id);
	    		return PDS_SUCCESS;
	    	}
	    }
	    return PDS_REC_NOT_FOUND;
    }
	return PDS_REPO_NOTOPEN;

*/


	fseek(ndx_fptr, 0L, SEEK_SET);
	if(pdsInfo.repo_status == 1){
		while(fread(index_array,sizeof(int),2,ndx_fptr)){

			if(index_array[0] == key) {
				offset = index_array[1];
				fseek(repo_fptr, offset,SEEK_SET);
				fread(c, sizeof(struct Contact), 1, repo_fptr);

				return PDS_SUCCESS;
				
			}
		}
		return PDS_REC_NOT_FOUND;

	}
	return PDS_REPO_NOTOPEN;
}


int pds_open(char *repo_name, char *ndx_name){

	if(pdsInfo.repo_status == 0){
		repo_fptr = fopen(repo_name, "rb+");
		ndx_fptr = fopen(ndx_name, "rb+");
		if(repo_fptr && ndx_fptr){
			pdsInfo.repo_status = 1;
			return PDS_SUCCESS;
		}
			
		else{
			pdsInfo.repo_status = 0;
			return PDS_OPEN_ERROR;
		}		
	} 
	else {
		return PDS_REPO_ALREADYOPEN;
	}

		
}

int pds_close(){
	fclose(repo_fptr);
	fclose(ndx_fptr);
	pdsInfo.repo_status = 0;

	return PDS_REPO_NOTOPEN;
}
/*
int pds_create_index( char *repo_name, char *ndx_name ) {
	repo_fptr = fopen(repo_name, "wb" );
	ndx_fptr = fopen(ndx_name, "wb");
	if(repo_fptr && ndx_fptr)
		return PDS_SUCCESS;
	else	
		return PDS_CREATE_ERROR;
}
*/
