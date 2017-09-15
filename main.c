#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

unsigned int hms2ms(char *hms_str){
	unsigned int ret = 0;
	int i, m[]={60*60*10,60*60,0,60*10,60,0,10,1};

	for(i=0;i<8;i++)
		ret += (hms_str[i]-48)*m[i];
	ret *= 1000;
	i = atoi(hms_str+9);
	ret += i;

	return ret;
}

int main(int argc, char *argv[]){
	int opt, r, c, ccount, bufi;

	int n;
	char t1[15],t2[15],buf[128];

	char *srt_fn = NULL;
	char *smi_fn = NULL;
	char *class_n = NULL;
	FILE *srt_fp, *smi_fp;

	while((opt=getopt(argc,argv,"f:t:c:"))!=-1){
	switch(opt){
	case 'f':
		srt_fn = strdup(optarg);
		break;
	case 't':
		smi_fn = strdup(optarg);
		break;
	case 'c':
		class_n = strdup(optarg);
		break;
	default:
		break;
	}
	}

	if(srt_fn == NULL || smi_fn == NULL || class_n == NULL)
		return -1;

	srt_fp = fopen(srt_fn,"r");
	smi_fp = fopen(smi_fn,"w");

	fprintf(smi_fp,"<sami>\n<head>\n<title>%s</title>\n<style></style>\n",argv[0]);
	fprintf(smi_fp,"</head>\n<body>\n");
	fprintf(smi_fp,"<sync start=0><p class=%s>\n",class_n);

	while(1){

		r = fscanf(srt_fp,"%d\n",&n);
		if(r!=1) break;

		fscanf(srt_fp,"%s --> %s\n",t1,t2);
		bufi=0; ccount=0;
		while(1){
			c = fgetc(srt_fp);
			if(c == '\n' && ccount ==0){
				buf[bufi-1]='\0';
				break;
			}
			if(ccount == 0){
				buf[bufi]='\0';
				strncat(buf,"<br>",4);
				bufi+=4;
			}
			buf[bufi++] = c;
			if(c == '\n') ccount = 0;
			else ccount++;
		}
		fprintf(smi_fp,"<sync start=%d>",hms2ms(t1));
		fprintf(smi_fp,"<p class=%s>\n",class_n);
		fprintf(smi_fp,"%s\n",buf);
		fprintf(smi_fp,"<sync start=%d>",hms2ms(t2));
		fprintf(smi_fp,"<p class=%s>&nbsp\n",class_n);
	}
	fclose(srt_fp);
	fprintf(smi_fp,"</body></sami>");
	fclose(smi_fp);
	return 0;
}
