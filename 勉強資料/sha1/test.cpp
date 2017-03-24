CSHA1 sha1;  
char str1[] = "123456";  
char str2[] = "456789abcd";  
sha1.Update((unsigned char*)str1,strlen(str1);  
sha1.Update((unsigned char*)str2, strlen(str2);  
sha1.Final();  
unsigned char chSha1[20] = "";  
sha1.GetHash(chSha1);  