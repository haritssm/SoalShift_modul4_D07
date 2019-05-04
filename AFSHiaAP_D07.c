#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>

static const char *dirpath = "/home/zicoritonda/shift4";

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

static int xmp_getattr(const char *path, struct stat *stbuf)
{
 	int res;
	char fpath[1000];
	int len = strlen(path);
	char *newpath = encode_CC((char *)path,len);
	sprintf(fpath,"%s%s",dirpath,newpath);

	res = lstat(fpath, stbuf);

	if (res == -1)
		return -errno;

	return 0;
}

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

static int xmp_read(const char *path, char *buf, size_t size, off_t offset,
		    struct fuse_file_info *fi)
{
  	char fpath[1000];
  	int len = strlen(path);
	char *newpath = encode_CC((char *)path,len);
	if(strcmp(newpath,"/") == 0)
	{
		newpath=(char *)dirpath;
		sprintf(fpath,"%s",newpath);
	}
	else sprintf(fpath, "%s%s",dirpath,newpath);
	int res = 0;
  int fd = 0 ;

	(void) fi;
	fd = open(fpath, O_RDONLY);
	if (fd == -1)
		return -errno;

	res = pread(fd, buf, size, offset);
	if (res == -1)
		res = -errno;

	close(fd);
	return res;
}

static int xmp_open(const char *path, struct fuse_file_info *fi)
{
    int res;
    int len = strlen(path);
	char *newpath = encode_CC((char *)path,len);
    res = open(newpath, fi->flags);
    if (res == -1)
            return -errno;
    fi->fh = res;
    return 0;
}

static int xmp_write(const char *path, const char *buf, size_t size,
                     off_t offset, struct fuse_file_info *fi)
{
    int fd;
    int res;
    //int len = strlen(path);
	//char *newpath = decode_CC((char *)path,len);
	char loc[1000];
	sprintf(loc,"%s%s",dirpath,path);
    (void) fi;
    if(fi == NULL)
            fd = open(loc, O_WRONLY);
    else
            fd = fi->fh;
        
    if (fd == -1)
            return -errno;
    res = pwrite(fd, buf, size, offset);
    if (res == -1)
            res = -errno;
    if(fi == NULL)
            close(fd);
    return res;
}

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
			//strcat(loc,".iz1");
			//res = open(loc, fi->flags, mode);
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

static int xmp_chown(const char *path, uid_t uid, gid_t gid)
{
        int res;
        int len = strlen(path);
		char *newpath = encode_CC((char *)path,len);
        res = lchown(newpath, uid, gid);
        if (res == -1)
                return -errno;
        return 0;
}

static int xmp_mkdir(const char *path, mode_t mode)
{
        int res;
        int len = strlen(path);
		char *newpath = encode_CC((char *)path,len);
		char loc[1000];
		sprintf(loc,"%s/%s",dirpath,newpath);
		res = mkdir(loc, mode);

		/*//soal4
		if(strcmp(newpath,"/@ZA>AXio")==0){//@ZA>AXio
			chmod(loc,705);
			//stbuf->st_mode = S_IFDIR | 0705;
			printf("Berhasil\n");
		}*/

        //res = mkdir(newpath, mode);
        if (res == -1)
                return -errno;
        return 0;
}


static int xmp_rmdir(const char *path)
{
        int res;
        int len = strlen(path);
		char *newpath = encode_CC((char *)path,len);
		char loc[1000];
		sprintf(loc,"%s/%s",dirpath,newpath);
        res = rmdir(loc);
        if (res == -1)
                return -errno;
        return 0;
}

/*static int xmp_mknod(const char *path, mode_t mode, dev_t rdev)
{
        int res;
        int len = strlen(path);
		char *newpath = encode_CC((char *)path,len);
		char loc[500];
		sprintf(loc,"%s%s",dirpath,newpath);
        if (S_ISREG(mode)) {
                res = open(loc, O_CREAT | O_EXCL | O_WRONLY, mode);
                if (res >= 0)
                        res = close(res);
        } else if (S_ISFIFO(mode))
                res = mkfifo(newpath, mode);
        else
                res = mknod(newpath, mode, rdev);
        if (res == -1)
                return -errno;
        return 0;
}*/

static int xmp_utimens(const char *path, const struct timespec ts[2])
{
        //(void) fi;
        int res;
        int len = strlen(path);
		char *newpath = encode_CC((char *)path,len);
		char loc[500];
		sprintf(loc,"%s%s",dirpath,newpath);
        res = utimensat(0, loc, ts, AT_SYMLINK_NOFOLLOW);
        if (res == -1)
                return -errno;
        return 0;
}

/*static int xmp_symlink(const char *from, const char *to)
{
        int res;
        res = symlink(from, to);
        if (res == -1)
                return -errno;
        return 0;
}*/

/*static int xmp_link(const char *from, const char *to)
{
        int res;
        res = link(from, to);
        if (res == -1)
                return -errno;
        return 0;
}*/

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

static struct fuse_operations xmp_oper = {
	.getattr	= xmp_getattr,
	.readdir	= xmp_readdir,
	.read		= xmp_read,
	.open 		= xmp_open,
	.write 		= xmp_write,
	.chmod 		= xmp_chmod,
	.chown 		= xmp_chown,
	.mkdir 		= xmp_mkdir,
	.rmdir 		= xmp_rmdir,
	//.mknod 		= xmp_mknod,
	.utimens 	= xmp_utimens,
	//.symlink 	= xmp_symlink,
	//.link 		= xmp_link,
	.create 		= xmp_create,
};

int main(int argc, char *argv[])
{
	umask(0);
	return fuse_main(argc, argv, &xmp_oper, NULL);
}
