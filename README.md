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

Jadi parameter untuk fungsi enkripsi dan dekripsi ini adalah string yang ingin diubah dan juga panjang stringnya. String akan diubah perkarakter dengan cara menambahkan dengan key dan akan mengubahnya dengan karater yang ada pada array listChar.

Karena sebelum dimount file terenkripsi maka kita harus dapat mendekripsinya. Untuk melakukan hal tersebut kami mengubah namanya saat filesistem melakukan fungsi readdir.
```
static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
		       off_t offset, struct fuse_file_info *fi)
{
  	char fpath[1000];
  	int len = strlen(path);
  	char *newpath = encode_CC((char *)path,len);
	if(strcmp(path,"/") == 0)
	{
		path=dirpath;
		sprintf(fpath,"%s",path);
	}
	else{
		sprintf(fpath, "%s%s",dirpath,newpath);
	}
	int res = 0;

	DIR *dp;
	struct dirent *de;

	(void) offset;
	(void) fi;

	dp = opendir(fpath);
	if (dp == NULL)
		return -errno;

	while ((de = readdir(dp)) != NULL) {
		if(strcmp(de->d_name,".")!=0 && strcmp(de->d_name,"..")!=0){
			int len = strlen(de->d_name);
			char *new = decode_CC(de->d_name,len); //soal 1

			//soal 3
			char loc[1000];
			sprintf(loc,"%s/%s",fpath,de->d_name);
			struct stat st, info;
			memset(&st, 0, sizeof(st));
			memset(&info, 0, sizeof(info));
			st.st_ino = de->d_ino;
			st.st_mode = de->d_type << 12;

			stat(loc,&info);
			struct passwd *pw = getpwuid(info.st_uid);
			struct group *gr = getgrgid(info.st_gid);
			char owner1[10] = "chipset";
			char owner2[15] = "ic_controller";
			char group[10] = "rusak";
			//printf("\n%s; %s; %s; %s;\n",new, pw->pw_name,gr->gr_name,ctime(&info.st_mtime));
			FILE *fl;
			if(strcmp(pw->pw_name,owner1)==0 || strcmp(pw->pw_name,owner2)==0 || strcmp(gr->gr_name,group)==0){
				char filemiris[50] = "/home/zicoritonda/shift4/";
				char *fm = encode_CC("filemiris.txt",13);
				strcat(filemiris,fm);
				printf("%s\n",filemiris);
				fl = fopen(filemiris,"a");
				fprintf(fl,"%s; %s; %s; %s",new,pw->pw_name,gr->gr_name,ctime(&info.st_mtime));
				printf("---File Berbahaya---\n");
				chmod(loc,777);
				remove(loc);
				fclose(fl);
			}

			//soal4
			printf("\n%s\n",fpath);
			char youtube[50] = "/home/zicoritonda/shift4/";
			char *yt = encode_CC("YOUTUBER",8);
			strcat(youtube,yt);
			printf("untuk soal no 4: %s\n",youtube);
			if(strcmp(fpath,youtube)==0){
				if(S_ISREG(info.st_mode)){
					printf("\nChange Mode of a File\n");
					//char newloc[1000];
					//sprintf(newloc,"%s%s.iz1",fpath,de->d_name);
					chmod(loc,640);
					//rename(loc,newloc);
				}
				if(S_ISDIR(info.st_mode)){
					printf("\nChange Mode of a Directory\n");
					chmod(loc,705);
				}
				//chmod(loc,705);
				//st.st_mode = S_IFDIR | 0705;
			}

			res = (filler(buf, new, &st, 0));
			if(res!=0) break;
		}
	}

	closedir(dp);
	return 0;
}
```
```
int len = strlen(de->d_name);
char *new = decode_CC(de->d_name,len); //soal 1
```

Karena ini hanya mengubah nama tampilanya saja dan file sebenarnya memiliki nama file yang berbeda, maka setiap kali kita akan mengaksesnya kita harus men enkripsinya kembali dan hal ini dilakukan pada fungsi lainnya.
```
int len = strlen(path);
char *newpath = encode_CC((char *)path,len);
sprintf(fpath,"%s%s",dirpath,newpath);
```

## Soal 3
Pada soal nomor 3, kita harus dapat mencek owner dan group dari file tersebut. Jika file tersebut memiliki owner "chipset" atau "ic_controller" atau group "rusak" ataupun file tidak dapat dibuka maka kita harus menghapus file tersebut dan memasukan nama file nya kedalam file bernama filemiris.txt

### Penyelesaian
Untuk dapat mengetahui owner file kita harus mengakses stat dari file tersebut.
```
stat(loc,&info);
struct passwd *pw = getpwuid(info.st_uid);
struct group *gr = getgrgid(info.st_gid);
```

Jika file memiliki owner atau group yang sesuai kriteria maka file akan dihapus dan dibuat filemiri.txt
```
char owner1[10] = "chipset";
char owner2[15] = "ic_controller";
char group[10] = "rusak";
FILE *fl;
if(strcmp(pw->pw_name,owner1)==0 || strcmp(pw->pw_name,owner2)==0 || strcmp(gr->gr_name,group)==0){
	char filemiris[50] = "/home/zicoritonda/shift4/";
	char *fm = encode_CC("filemiris.txt",13);
	strcat(filemiris,fm);
	printf("%s\n",filemiris);
	fl = fopen(filemiris,"a");
	fprintf(fl,"%s; %s; %s; %s",new,pw->pw_name,gr->gr_name,ctime(&info.st_mtime));
	printf("---File Berbahaya---\n");
	chmod(loc,777);
	remove(loc);
	fclose(fl);
}
```

## Soal 4
Pada soal nomor 4, pada saat kita membuat file atau folder kita harus dapat mengganti mode folder menjadi 705 dan mode file menjadi 640 dan juga menambahkan ekstensi .iz1 jika folder dan file tersebut berada dalam folder YOUTUBER pada filesystem.

### Penyelesaian
Pertama kita harus mencek apakah sedang berada pada folder YOUTUBER. Setelah itu kita cek apakah merupakan folder ataupun file. Jika folder kita ganti modenya menjadi 705 dan file menjadi 640.
```
char youtube[50] = "/home/zicoritonda/shift4/";
char *yt = encode_CC("YOUTUBER",8);
strcat(youtube,yt);
if(strcmp(fpath,youtube)==0){
	if(S_ISREG(info.st_mode)){
		printf("\nChange Mode of a File\n");
		chmod(loc,640);
	}
	if(S_ISDIR(info.st_mode)){
		printf("\nChange Mode of a Directory\n");
		chmod(loc,705);
	}
}
```

Untuk dapat mengganti ekstemsi dari file yang telah dibuat kami mengganti nama filenya pada fungsi xmp_create.
```
static int xmp_create(const char *path, mode_t mode,
                      struct fuse_file_info *fi)
{
        int res;
        int len = strlen(path);
		char *newpath = encode_CC((char *)path,len);
		char loc[500];
		sprintf(loc,"%s%s",dirpath,newpath);
        //res = open(loc, fi->flags, mode);
	
		//soal4
        char *point;
	if((point = strstr(newpath,"YOUTUBER")) != NULL){
		strcat(loc,".iz1");
		res = open(loc, fi->flags, mode);			
	}
	else res = open(loc, fi->flags, mode);

        if (res == -1)
                return -errno;
        fi->fh = res;
        return 0;
}
```

Untuk file dengan ekstensi .iz1 tidak dapat diubah modenya. Jika mode diubah akan memunculkan suatu  pesan bahwa mode file tidak dapat diubah. Untuk melakukannya kami menggunakan zenity dan memanggilnya pada fungsi chmod dengan cara mencek extensinya.
```
static int xmp_chmod(const char *path, mode_t mode)
{
        int res;
        int len = strlen(path);
		char *newpath = encode_CC((char *)path,len);
		char loc[1000];
		sprintf(loc,"%s/%s",dirpath,newpath);
	
		//soal4
		char *point;
		if((point = strstr(newpath,".iz1")) != NULL){
			printf("\nTidak bisa ganti\n");
			char *argv[5] = {"zenity","--error","--text","File ekstensi iz1 tidak boleh diubah permissionnya.",NULL};
			execv("/usr/bin/zenity",argv);
		}
		else res = chmod(loc, mode);

        res = chmod(loc, mode);
        if (res == -1)
                return -errno;
        return 0;
}
```
