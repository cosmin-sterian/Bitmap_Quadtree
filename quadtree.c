#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//int debug=0;

typedef struct QuadtreeNode{
	unsigned char blue, green, red, reserved;
	uint32_t area;
	int32_t bottom_left, top_left;
	int32_t bottom_right, top_right;
} __attribute__((packed)) QuadtreeNode;

/*typedef struct QuadtreeNode{
	unsigned char blue, green, red, reserved;
	uint32_t area;
	int32_t top_left, top_right;
	int32_t bottom_left, bottom_right;
} __attribute__((packed)) QuadtreeNode;*/

typedef struct QuadtreeNode2{
	unsigned char *blue, *green, *red, *reserved;
	uint32_t *area;
	int32_t *top_left, *top_right;
	int32_t *bottom_left, *bottom_right;
} __attribute__((packed)) QuadtreeNode2;


typedef struct FileHeader{
	short signature;
	int file_size, reserved, offset;
} FileHeader;

typedef struct InfoHeader{
	int size;
	int width, height;
	short planes, bit_count;
	int compression, image_size, x_pixels_per_meter, y_pixels_per_meter, colors_used, colors_important;
} InfoHeader;

typedef struct pixel{
	unsigned char blue, green, red, reserved;
} pixel;

typedef struct arbore{
	QuadtreeNode *nod;
	struct arbore *left1, *left2, *right1, *right2;
} arbore;

typedef struct list{
	arbore *val;
	struct list *next;
} list;

int isUniform(pixel ***a, int l, int r, int u, int d)
{
	int i,j;
	pixel *aux,*aux2;
	//printf("yolo in, L=%d, R=%d, U=%d, D=%d\n",l,r,u,d);
	for(i=u;i<d;i++)
	{
		for(j=l;j<r;j++)
		{
			if(i==u && j==l)
				aux=a[i][j];
			else
			{
				aux2=a[i][j];
				if(!(aux->blue==aux2->blue && aux->green==aux2->green && aux->red==aux2->red && aux->reserved==aux2->reserved))
				{
					//printf("Not Uniform\n");
					return 0;
				}
			}
			aux=a[i][j];
		}
	}
	//printf("Uniform\n");
	return 1;
}

void init(arbore **head)
{
	*head=NULL;
}

/*void insert(arbore **head, QuadtreeNode *val)
{
	arbore *new=malloc(sizeof(arbore));
	new->nod=val;
	new->left1=NULL;
	new->left2=NULL;
	new->right1=NULL;
	new->right2=NULL;
	if(*head==NULL)
	{
		*head=new;
	}
	else
	{
		arbore *aux=*head;
		arbore *prev=aux;
		while(aux!=NULL)
		{
			prev=aux;
			if(aux->left1==NULL)
				aux=aux->left1;
			else
			{
				if(aux->left2==NULL)
					aux=aux->left2;
				else
				{
					if(aux->right1==NULL)
						aux=aux->right1;
					else
					{
						if(aux->right2==NULL)
							aux=aux->right2;
					}
				}
			}
		}
	}
}*/

/*arbore *ceva(pixel ***a, int l, int r, int u, int d)
{
	arbore *new=malloc(sizeof(arbore));
	//printf("*********PASSED**********\n");
	new->nod=malloc(sizeof(QuadtreeNode));
	//printf("*********PASSED**********\n");
	new->left1=NULL;
	new->left2=NULL;
	new->right1=NULL;
	new->right2=NULL;
	//printf("*********PASSED**********\n");
	printf("Left=%d, Right=%d, Up=%d, Down=%d\n",l,r,u,d);
	new->nod->area=(r-l+1)*(r-l+1);
	if((l==r && r==u && u==d) || isUniform(a,l,r,u,d))
	{
		//new->nod=malloc(sizeof(QuadtreeNode));
		new->nod->blue=a[u][u]->blue;
		new->nod->green=a[u][u]->green;
		new->nod->red=a[u][u]->red;
		new->nod->reserved=a[u][u]->reserved;
	}
	else
	{
		new->left1=ceva(a,l,r/2,u,d/2);
		new->left2=ceva(a,l/2+1,r,u,d/2);
		new->right1=ceva(a,l/2+1,r,u/2+1,d);
		new->right2=ceva(a,l,r/2,u/2+1,d);
	}
	return new;
}*/

arbore *ceva(pixel ***a, int l, int r, int u, int d)
{
	arbore *root=malloc(sizeof(arbore));
	//root->nod=NULL;
	root->nod=malloc(sizeof(QuadtreeNode));
	root->nod->blue=0;
	root->nod->green=0;
	root->nod->red=0;
	root->nod->reserved=0;
	root->left1=NULL;
	root->left2=NULL;
	root->right1=NULL;
	root->right2=NULL;
	root->nod->area=(r-l+1)*(r-l+1);
	root->nod->bottom_left=-1;
	root->nod->bottom_right=-1;
	root->nod->top_left=-1;
	root->nod->top_right=-1;
	//printf("Pasul: %d\n",debug++);
	if(isUniform(a,l,r,u,d))
	{
		//printf("hey\n");
		//root->nod=malloc(sizeof(QuadtreeNode));
		root->nod->blue=a[l][l]->blue;
		root->nod->green=a[l][l]->green;
		root->nod->red=a[l][l]->red;
		root->nod->reserved=a[l][l]->reserved;
		root->left1=NULL;
		root->left2=NULL;
		root->right1=NULL;
		root->right2=NULL;
		root->nod->bottom_left=-1;
		root->nod->bottom_right=-1;
		root->nod->top_left=-1;
		root->nod->top_right=-1;
		return root;
	}
	else
	{
		root->left1=ceva(a,l,(l+r)/2,u,(u+d)/2);
		//printf("left1 assigned\n");
		//root->left2=ceva(a,(l+r)/2+1,r,u,(u+d)/2);
		root->left2=ceva(a,(l+r)/2,r,u,(u+d)/2);
		//printf("left2 assigned\n");
		//root->right1=ceva(a,(l+r)/2+1,r,(u+d)/2+1,d);
		root->right1=ceva(a,(l+r)/2,r,(u+d)/2,d);
		//printf("right1 assigned\n");
		//root->right2=ceva(a,l,(l+r)/2,(u+d)/2+1,d);
		root->right2=ceva(a,l,(l+r)/2,(u+d)/2,d);
		//printf("right2 assigned\n");
		return root;
	}
}

uint32_t countFrunze(arbore *head)
{
	uint32_t k=0;
	if(head->left1==NULL && head->left2==NULL && head->right1==NULL && head->right2==NULL)
		return 1;
	else
	{
		k=countFrunze(head->left1)+countFrunze(head->left2)+countFrunze(head->right1)+countFrunze(head->right2);
		return k;
	}
}

uint32_t countNoduri(arbore *head)
{
	uint32_t k=0;
	if(head==NULL)
		return 0;
	else
	{
		k=1+countNoduri(head->left1)+countNoduri(head->left2)+countNoduri(head->right1)+countNoduri(head->right2);
		return k;
	}
}

void initL(list **head)
{
	*head=NULL;
}

void insertL(list **head, arbore *val)
{
	list *new=malloc(sizeof(list));
	new->next=NULL;
	new->val=val;
	if(*head==NULL)
		*head=new;
	else
	{
		list *aux=*head;
		while(aux->next)
			aux=aux->next;
		aux->next=new;
	}
}

void freeL(list **head)
{
	list *aux=*head;
	while(*head)
	{
		*head=(*head)->next;
		free(aux);
		aux=(*head);
	}
}

arbore *pop(list **head)
{
	if(*head)
	{
		list *aux=*head;
		*head=(*head)->next;
		return aux->val;
	}
	else
		return NULL;
}

/*void freeArbore(arbore *root)
{

	printf("Pas: %d\n",debug++);
	arbore *aux;
	list *lista;
	initL(&lista);
	insertL(&lista,root);
	while(lista)
	{
		printf("Pas: %d\n",debug++);
		aux=pop(&lista);
		if(aux->left1!=NULL)
			insertL(&lista,aux->left1);
		if(aux->left2!=NULL)
			insertL(&lista,aux->left2);
		if(aux->right1!=NULL)
			insertL(&lista,aux->right1);
		if(aux->right2!=NULL)
			insertL(&lista,aux->right2);
		printf("1\n");
		free(aux->nod);
		printf("2\n");
		//free(aux);
		//printf("3\n");
	}
	freeL(&lista);
}*/

/*void freeArbore(arbore *root)
{
	if(root==NULL)
		return;
	else
	{
		if(root->left1!=NULL)
			freeArbore(root->left1);
		if(root->left2!=NULL)
			freeArbore(root->left2);
		if(root->right1!=NULL)
			freeArbore(root->right1);
		if(root->right2!=NULL)
			freeArbore(root->right2);
		if(root->nod!=NULL)
		{
			free(root->nod);
			root->nod=NULL;
		}
		free(root);
		root=NULL;
		return;
	}
}*/

void freeArbore(arbore **root)
{
	arbore *aux;
	list *head;
	initL(&head);
	insertL(&head,(*root));
	while(head)
	{
		aux=pop(&head);
		if(aux!=NULL)
		{
			if(aux->left1!=NULL)
		        insertL(&head, aux->left1);
			if(aux->left2!=NULL)
		        insertL(&head, aux->left2);
		        if(aux->right1!=NULL)
		        insertL(&head, aux->right1);
			if(aux->right2!=NULL)
		        insertL(&head, aux->right2);
			/*if(aux->nod!=NULL)
			{
				free(aux->nod);
				aux->nod=NULL;
			}*/
			free(aux);
			aux=NULL;
		}
	}
	*root=NULL;
	freeL(&head);
}

arbore **bfs(arbore *root, list *head, uint32_t numar_noduri)
{
	arbore *aux;
	arbore **v=malloc(numar_noduri*sizeof(arbore*));
	int i=0;
	insertL(&head,root);
	while(head)
	{
		aux=pop(&head);
		//printf("%d ",aux2->val);
		//v[i].nod=malloc(sizeof(QuadtreeNode));
		v[i++]=aux;
	        if(aux->left1!=NULL)
	        insertL(&head, aux->left1);
		if(aux->left2!=NULL)
	        insertL(&head, aux->left2);
	        if(aux->right1!=NULL)
	        insertL(&head, aux->right1);
		if(aux->right2!=NULL)
	        insertL(&head, aux->right2);
	        //free(aux2);
		//free(aux);
	        //aux=NULL;
	}
	freeL(&head);
	return v;
}

/*int cmpArbore(arbore a, arbore b)
{
	if(a.nod->blue == b.nod->blue && a.nod->green == b.nod->green && a.nod->red == b.nod->red && a.nod->reserved == b.nod->reserved
		&& a.nod->area == b.nod->area && a.nod->top_left == b.nod->top_left && a.nod->top_right == b.nod->top_right
		&& a.nod->bottom_right == b.nod->bottom_right && a.nod->bottom_left == b.nod->bottom_left && a.left1 == b.left1
		&& a.left2 == b.left2 && a.right1 == b.right1 && a.right2 == b.right2)
		return 1;
	else
		return 0;
}*/

QuadtreeNode *indexing(arbore **v, uint32_t numar_noduri)
{
	uint32_t i,j;
	for(i=0;i<numar_noduri-1;i++)
	{
		for(j=i+1;j<numar_noduri;j++)
		{
			if(v[i]->left1==v[j])
				v[i]->nod->top_left=j;
			if(v[i]->left2==v[j])
				v[i]->nod->top_right=j;
			if(v[i]->right1==v[j])
				v[i]->nod->bottom_right=j;
			if(v[i]->right2==v[j])
				v[i]->nod->bottom_left=j;
		}
		//printf("V[%d]: top_left=%d, top_right=%d, bottom_right=%d, bottom_left=%d\n",i,v[i]->nod->top_left,v[i]->nod->top_right,v[i]->nod->bottom_right,v[i]->nod->bottom_left);
	}
	QuadtreeNode *vector=malloc(numar_noduri*sizeof(QuadtreeNode));
	for(i=0;i<numar_noduri;i++)
	{
		vector[i].blue=v[i]->nod->blue;
		vector[i].green=v[i]->nod->green;
		vector[i].red=v[i]->nod->red;
		vector[i].reserved=v[i]->nod->reserved;
		vector[i].area=v[i]->nod->area;
		vector[i].top_left=v[i]->nod->top_left;
		vector[i].top_right=v[i]->nod->top_right;
		vector[i].bottom_right=v[i]->nod->bottom_right;
		vector[i].bottom_left=v[i]->nod->bottom_left;
		free(v[i]->nod);
		free(v[i]);
	}
	free(v);
	return vector;
}

arbore *task2(QuadtreeNode *v, int32_t k)
{
	arbore *new=malloc(sizeof(arbore));
	new->left1=NULL;
	new->left2=NULL;
	new->right1=NULL;
	new->right2=NULL;
	new->nod=malloc(sizeof(QuadtreeNode));
	//printf("%f\n",(unsigned char)v[k].blue);
	new->nod->blue=v[k].blue;
	new->nod->green=v[k].green;
	new->nod->red=v[k].red;
	new->nod->reserved=v[k].reserved;
	new->nod->area=v[k].area;
	new->nod->bottom_left=v[k].bottom_left;
	new->nod->top_left=v[k].top_left;
	new->nod->top_right=v[k].top_right;
	new->nod->bottom_right=v[k].bottom_right;
	//printf("Vector[%d]: bottom_left=%d, top_left=%d, top_right=%d, bottom_right=%d\n",k,v[k].bottom_left,v[k].top_left,v[k].top_right,v[k].bottom_right);
	//printf("Vector[%d]: bottom_left=%d, top_left=%d, top_right=%d, bottom_right=%d\n",k,(int32_t)v[k].bottom_left,(int32_t)v[k].top_left,(int32_t)v[k].top_right,(int32_t)v[k].bottom_right);
	//printf("Vector[%d]: bottom_left=%d, top_left=%d, top_right=%d, bottom_right=%d\n",2,v[2].bottom_left,v[2].top_left,v[2].top_right,v[2].bottom_right);
	//printf("Vector[%d]: bottom_left=%d, top_left=%d, top_right=%d, bottom_right=%d\n",1,v[1].bottom_left,v[1].top_left,v[1].top_right,v[1].bottom_right);

	if(v[k].bottom_right==-1 || v[k].bottom_left==-1 || v[k].top_right==-1 || v[k].top_left==-1)
		return new;
	else
	{
		new->left1=task2(v, (int32_t)v[k].top_left);
		new->left2=task2(v, (int32_t)v[k].top_right);
		new->right1=task2(v, (int32_t)v[k].bottom_right);
		new->right2=task2(v, (int32_t)v[k].bottom_left);
		return new;
	}
}

void task2Matr(arbore *head, pixel **a, int l, int r, int u, int d)
{
	if(head->nod->bottom_left==-1)
	{
		int i,j;
		for(i=u;i<d;i++)
			for(j=l;j<r;j++)
			{
				a[i][j].blue=head->nod->blue;
				a[i][j].green=head->nod->green;
				a[i][j].red=head->nod->red;
				a[i][j].reserved=head->nod->reserved;
				//printf("%c %c %c %c\n",a[i][j].blue, a[i][j].green, a[i][j].red, a[i][j].reserved);
			}
		free(head->nod);
		free(head);
		return;
	}
	else
	{
		task2Matr(head->left1, a, l, (l+r)/2, u, (u+d)/2);
		//task2Matr(head->left2, a, (l+r)/2+1, r, u, (u+d)/2);
		task2Matr(head->left2, a, (l+r)/2, r, u, (u+d)/2);
		//task2Matr(head->right1, a, (l+r)/2+1, r, (u+d)/2+1, d);
		task2Matr(head->right1, a, (l+r)/2, r, (u+d)/2, d);
		//task2Matr(head->right2, a, l, (l+r)/2, (u+d)/2+1, d);
		task2Matr(head->right2, a, l, (l+r)/2, (u+d)/2, d);
		return;
	}
}

int main(int argc, char *argv[])
{
	FileHeader *file_header=malloc(sizeof(FileHeader));
	InfoHeader *info_header=malloc(sizeof(InfoHeader));
	FILE *file=fopen(argv[argc-2],"rb");
	FILE *out=fopen(argv[argc-1],"wb");
	//----------------Citire File Header------------------------------------
	fread(&file_header->signature, sizeof(short), 1, file);
	fread(&file_header->file_size, sizeof(int), 1, file);
	fread(&file_header->reserved, sizeof(int), 1, file);
	fread(&file_header->offset, sizeof(int), 1, file);

	//----------------Citire Info Header------------------------------------
	fread(&info_header->size, sizeof(int), 1, file);
	fread(&info_header->width, sizeof(int), 1, file);
	fread(&info_header->height, sizeof(int), 1, file);
	fread(&info_header->planes, sizeof(short), 1, file);
	fread(&info_header->bit_count, sizeof(short), 1, file);
	fread(&info_header->compression, sizeof(int), 1, file);
	fread(&info_header->image_size, sizeof(int), 1, file);
	fread(&info_header->x_pixels_per_meter, sizeof(int), 1, file);
	fread(&info_header->y_pixels_per_meter, sizeof(int), 1, file);
	fread(&info_header->colors_used, sizeof(int), 1, file);
	fread(&info_header->colors_important, sizeof(int), 1, file);

	//---------------Scriere File Header------------------------------------
	fwrite(&file_header->signature, sizeof(short), 1, out);
	fwrite(&file_header->file_size, sizeof(int), 1, out);
	fwrite(&file_header->reserved, sizeof(int), 1, out);
	fwrite(&file_header->offset, sizeof(int), 1, out);

	//----------------Scriere Info Header-----------------------------------
	fwrite(&info_header->size, sizeof(int), 1, out);
	fwrite(&info_header->width, sizeof(int), 1, out);
	fwrite(&info_header->height, sizeof(int), 1, out);
	fwrite(&info_header->planes, sizeof(short), 1, out);
	fwrite(&info_header->bit_count, sizeof(short), 1, out);
	fwrite(&info_header->compression, sizeof(int), 1, out);
	fwrite(&info_header->image_size, sizeof(int), 1, out);
	fwrite(&info_header->x_pixels_per_meter, sizeof(int), 1, out);
	fwrite(&info_header->y_pixels_per_meter, sizeof(int), 1, out);
	fwrite(&info_header->colors_used, sizeof(int), 1, out);
	fwrite(&info_header->colors_important, sizeof(int), 1, out);


	if(strcmp(argv[1],"-c")==0)
	{
		//----------------Citire Matrice----------------------------------------
		//printf("Citire Headere PASSED\n");
		int i,j;
		pixel ***a=malloc(info_header->height*sizeof(pixel**));
		for(i=info_header->height-1;i>=0;i--)
		{
			a[i]=malloc(info_header->width*sizeof(pixel*));
			for(j=0;j<info_header->width;j++)
			{
				a[i][j]=malloc(sizeof(pixel));
				fread(&a[i][j]->blue, sizeof(unsigned char), 1, file);
				fread(&a[i][j]->green, sizeof(unsigned char), 1, file);
				fread(&a[i][j]->red, sizeof(unsigned char), 1, file);
				fread(&a[i][j]->reserved, sizeof(unsigned char), 1, file);
			}
		}
		//printf("Citire Matrice PASSED\n");
		//printf("Closed input file\n");

		uint32_t numar_culori, numar_noduri;
		arbore *head;
		//printf("W:%d H:%d\n",info_header->width,info_header->height);
		//printf("Beggining \"ceva\"\n");
		head=ceva(a,0,info_header->width,0,info_header->height);
		//printf("Ending \"ceva\"\n");
		numar_culori=countFrunze(head);
		numar_noduri=countNoduri(head);

		//printf("Numar_culori=%d, Numar_noduri=%d\n",numar_culori,numar_noduri);

		list *lista;
		initL(&lista);
		arbore **v=bfs(head, lista, numar_noduri);
		QuadtreeNode *vector=indexing(v, numar_noduri);


		fwrite(&numar_culori, sizeof(uint32_t), 1, out);
		fwrite(&numar_noduri, sizeof(uint32_t), 1, out);
		uint32_t k;
		for(k=0;k<numar_noduri;k++)
		{
			//printf("Vector[%d]: bottom_left=%d, top_left=%d, top_right=%d, bottom_right=%d\n",k,vector[k].bottom_left,vector[k].top_left,vector[k].top_right,vector[k].bottom_right);
			fwrite(&vector[k], sizeof(QuadtreeNode), 1, out);
			//free(vector[k]);
		}

		free(vector);
		freeL(&lista);
		//debug=0;
		//printf("PAss\n");
		//freeArbore(&head);
		//printf("PAss\n");
		for(i=info_header->height-1;i>=0;i--)
		{
			for(j=0;j<info_header->width;j++)
			{
				free(a[i][j]);
			}
			free(a[i]);
		}
		free(a);
	}
	if(strcmp(argv[1],"-d")==0)
	{
		uint32_t numar_noduri, numar_culori, k;
		fread(&numar_culori, sizeof(uint32_t), 1, file);
		fread(&numar_noduri, sizeof(uint32_t), 1, file);
		QuadtreeNode *vector = malloc((numar_noduri) * sizeof(QuadtreeNode));
		for(k=0;k<numar_noduri;k++)
		{
			fread(&vector[k], sizeof(QuadtreeNode), 1, file);
			//printf("Vector[%d]: bottom_left=%d, top_left=%d, top_right=%d, bottom_right=%d\n",k,vector[k].bottom_left,vector[k].top_left,vector[k].top_right,vector[k].bottom_right);
		}
		arbore *head;
		head=task2(vector, 0);
		free(vector);
		pixel **a=malloc(info_header->height * sizeof(pixel*));
		int i, j;
		for(i=0;i<info_header->height;i++)
		{
			a[i]=malloc(info_header->width * sizeof(pixel));
		}
		task2Matr(head, a, 0, info_header->width, 0, info_header->height);
		for(i=info_header->height-1;i>=0;i--)
		{
			for(j=0;j<info_header->width;j++)
			{
				//printf("i:%d j:%d | %d %d %d %d | %c %c %c %c\n",i,j,a[i][j].blue,a[i][j].green,a[i][j].red,a[i][j].reserved,a[i][j].blue,a[i][j].green,a[i][j].red,a[i][j].reserved);
				fwrite(&a[i][j], sizeof(pixel), 1, out);
			}
		}

		for(i=0;i<info_header->height;i++)
		{
			free(a[i]);
		}
		free(a);
		//freeArbore(&head);
	}
	free(file_header);
	free(info_header);
	fclose(file);
	fclose(out);
	return 0;
}
