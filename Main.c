#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parseur.c"
#include "networkManager.c"
#include "globaleVar.h"

extern struct Action *actions;
extern struct Tache *taches;

int main(void)
{

	
    Parse();
    printf("nbActions : %d\n", nbActions);
    printf("nbTache : %d\n", nbTaches);

    printf("%s\n", actions[1].url);
    printf("%s\n", taches[1].nom);


    char *url = actions[1].url;
    char *outputFilePath = "/Users/mohameddennoun/Desktop/ProjetC/scrappeur/Websites/Google/google.html";
    HtmlDownload(url,outputFilePath);
    char *cmd = "curl https://www.pexels.com/ | grep img | grep -o src=http.* | grep -o http.* | sed 's/\?.*//' > ./Websites/Google/googleLinks.txt";
    // system(cmd);

    char *imageUrl = "https://unicornify.pictures/avatar/51d623f33f8b83095db84ff35e15dbe8?s=128";

    char* pictureFname = ImageFromHtml(outputFilePath);
    printf("%s\n", pictureFname);
    if(pictureFname != NULL) {
        DownloadImages(pictureFname, "https://www.google.com", sizeof("https://www.google.com"));
    }
    return 0;
}’’