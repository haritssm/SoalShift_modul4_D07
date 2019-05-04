# SoalShift_modul4_D07
Pada soal shift kali ini, kita diharapkan untuk dapat membuat suatu file system yang sesuai kebutuhan soal shift.

## Soal 1
Pada soal nomor 1, kita diharapkan dapat membuat file system kita dapat melakukan dekripsi pada file maupun folder yang awalnya telah di enkripsi menggunakan enkripsi caesar chipher yang telah dimodifikasi sesuai soal. Pada saat sebelum di mount file terenkripsi dan setelahnya nama file dan foldernya telah di dekripsi.

### Penyelesaian
Untuk dapat melakukan enkripsi sesuai soal, kami harus membuat sendiri enkripsi caesar ciphernya karena memiliki ketentuan sendiri.
```
int key = 17;
char *encode_CC(char str[],int len){//decode caesar cipher ; key:17
	char *result = (char *) malloc(len);
	char c,temp[100];
	int i,j;
	int listChar[94]={113,69,49,126,32,89,77,85,82,50,
					  34,96,104,78,73,100,80,122,105,37,
					  94,116,64,40,65,111,58,61,67,81,
					  44,110,120,52,83,91,55,109,72,70,
					  121,101,35,97,84,54,43,118,41,68,
					  102,75,76,36,114,63,98,107,79,71,
					  66,62,125,33,57,95,119,86,39,93,
					  106,99,112,53,74,90,38,88,108,124,
					  92,56,115,59,103,60,123,51,46,117,
					  42,87,45,48};//ascii
	i=0;
	while(i<=len){
		for(j=0;j<94;j++){
			if(str[i] == listChar[j]){
				if(j+key>93)j-=94;
				c = listChar[j+key];
				temp[i]=c;
				break;
			}
			else temp[i] = str[i];
		}
		i++;
	}
	temp[len] = '\0';
	strcpy(result,temp);
	//printf("%s\n",result);
	return result;
}

char *decode_CC(char str[],int len){//decode caesar cipher ; key:17
	char *result = (char *) malloc(len);
	char c,temp[100];
	int i,j;
	int listChar[94]={113,69,49,126,32,89,77,85,82,50,
					  34,96,104,78,73,100,80,122,105,37,
					  94,116,64,40,65,111,58,61,67,81,
					  44,110,120,52,83,91,55,109,72,70,
					  121,101,35,97,84,54,43,118,41,68,
					  102,75,76,36,114,63,98,107,79,71,
					  66,62,125,33,57,95,119,86,39,93,
					  106,99,112,53,74,90,38,88,108,124,
					  92,56,115,59,103,60,123,51,46,117,
					  42,87,45,48};//ascii
	i=0;
	while(i<=len){
		for(j=0;j<94;j++){
			if(str[i] == listChar[j]){
				if(j-key<0)j+=94;
				c = listChar[j-key];
				temp[i]=c;
				break;
			}
			else temp[i] = str[i];
		}
		i++;
	}
	temp[len] = '\0';
	strcpy(result,temp);
	//printf("%s\n",result);
	return result;
}
```
