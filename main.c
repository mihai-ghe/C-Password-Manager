/*
 * main.c
 *
 *  Created on: Feb 28, 2022
 *      Author: th3m4tr1xhasy0uf0llow1ngthewhit3r4bbit
 */
	#include <stdio.h>
	#include "commons.h"
	#include <GL/glut.h>
	#include <sys/stat.h>

	int *v_arr, n; float *gl_arr, *color_arr;

	void Display() {


		glClearColor(0.15f, 0.15f, 0.15f, 0.5f);
		glClear(GL_COLOR_BUFFER_BIT);

		int i; float lc, rc;
		float baseline = -0.3;
		for(i = 0; i < n; i++) {

			lc = (float) -0.95 + i*(1.9/n);
			rc = (float) lc + (2.0/n) -0.15*(1.9/n);

			glBegin(GL_QUADS);
			glColor3f(color_arr[i], 1 - color_arr[i], 0);
			glVertex2f(lc, baseline);
			glVertex2f(rc, baseline);
			glVertex2f(rc, baseline + gl_arr[i]);
			glVertex2f(lc, baseline + gl_arr[i]);
			glEnd();

			glRasterPos2f(lc + 0.42 * (1.9/n), baseline + gl_arr[i] + 0.05);
			if(v_arr[i] / 10) {
				glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, v_arr[i] / 10 +'0');
				glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, v_arr[i] % 10 +'0');
			}
			else
				glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, v_arr[i] +'0');
		}
		glFlush();
	}

	typedef struct {

		char ckey[17];
		char fname[41];
		char dfname[42];
		char bfname[41];
		char iv[17];

	}user_data;

	void Create_User();

	int check_pass(char *, char *, char *);

	int Authenticate(char *, char *, char *);

	void Add_Entry(user_data);

	void Ret_All(user_data);

	void Ret_Spec(user_data);

	void Show_Graph(user_data);

	void Modify_Entry(user_data);

	void Del_Entry(user_data);

	int CA_Screen(user_data *udat) {
		char option;
		int authenticated = 0;
		char ckey[17];
		char fname[41];
		char bfname[41];

		printf("\nWelcome to PassShield Password Manager!");
		while(!authenticated) {

			printf("\n\n1. Authenticate");

			printf("\n2. Create User");

			printf("\n3. Exit");

			printf("\nSelect an option to continue: ");
			fflush(stdout);

			option = getc(stdin);
			getc(stdin);

			switch(option) {

			case '1':
				authenticated = Authenticate(ckey, fname, bfname);
				strcpy(udat->ckey, ckey); strcpy(udat->fname, fname);
				strcpy(udat->bfname, bfname); break; // Authentication function

			case '2': Create_User(); break; // User Creation function

			case '3': printf("\nExiting..."); authenticated = -1; return authenticated;

			default: printf("\nYou did not enter a valid value!\n"); continue; //If value isn't valid, loop

			}
		}

		return authenticated;
	}

	void Auth_Screen(int *authenticate, user_data *udat) {

		char option;

		printf("\n");
		fflush(stdout);

		while(authenticate) {

			printf("\nWhat would you like to do?");

			printf("\n1. Add a password");

			printf("\n2. Retrieve all passwords");

			printf("\n3. Retrieve passwords for a specific service");

			printf("\n4. Modify an entry ( service & password )");

			printf("\n5. Delete a password");

			printf("\n6. Show Password length graph -- Warning -- The program will terminate upon closing the graph window!");

			printf("\n7. Back (Logout)\n");

			printf("Select an option to continue: ");

			fflush(stdout);

			option = getc(stdin);
			getc(stdin);

			switch(option) {

				case '1': Add_Entry(*udat); break;

				case '2': Ret_All(*udat); break;

				case '3': Ret_Spec(*udat); break;

				case '4': Modify_Entry(*udat); break;

				case '5': Del_Entry(*udat); break;

				case '6': Show_Graph(*udat); break;

				case '7': *authenticate = 0; return;

				default: printf("\nOption Not Valid!\n"); continue;

			}
		}
	}

	int Authenticate(char *key, char *fname, char *bfname) {


		int authenticated = 0, try = 0;

		FILE *udf, *bf; //user data file, backup file

		char user[25], buf[123], passphrase[49];
		char *fuser, *fpasshash, *fsalt;

		printf("\n");
		fflush(stdout);

		while(1) {

			try ++;

			if((udf = fopen("Users.dat", "r")))
				break;

			if(try > 2) {

				printf("User data file not found or corrupted\n");
				fflush(stdout);
				break;
			}
		}

		if(!udf) {

			printf("Attempting recovery from backup...\n\n");
			fflush(stdout);

			try = 0;

			while(1) {

				try++;

				if((bf = fopen("Users.bak", "r")))
					break;

				if(try > 2) {

					printf("Backup file not found or corrupted...\n");
					printf("If this is the first time you are using the app, make sure to first create a user before attempting to authenticate!\n");
					return authenticated;
				}

			}

			fclose(bf);

			File_Copy("Users.bak", "Users.dat");

			printf("Recovery Successful!\n");
			fflush(stdout);

			udf = fopen("Users.dat", "r");
		}

		while(1) {

			printf("Enter your User Name: ");
			fflush(stdout);

			fgets(user, 25, stdin);
			user[strlen(user) - 1] = '\0';

			while(!feof(udf)) {

				fscanf(udf, "%s", buf);

				fuser = strtok(buf, ":");

				fpasshash = strtok(NULL, ".");

				fsalt = strtok(NULL, "\n");

				if(!(strcmp(fuser, user))) {

					try = 0;

					echo_off();

					while(try < 4) {

						try++;

						printf("Enter your Pass Phrase: ");
						fflush(stdout);

						fgets(passphrase, 49, stdin);
						passphrase[strlen(passphrase) - 1] = '\0';

						if((check_pass(fpasshash, fsalt, passphrase))) {

							authenticated = 1;
							for(int i = 0; i < 8; i++)
								sprintf(key + (i*2),"%02x", i % 2 ? passphrase[strlen(passphrase)-i-1] : passphrase[i]);
							strcpy(fname, ".\\User_Data\\");
							strcpy(bfname, ".\\User_Data\\");
							strcat(fname, user);
							strcat(bfname, user);
							strcat(fname, ".dat");
							strcat(bfname, ".bak");
							break;
						}
						else {

							printf("Password does not match!\n");
							fflush(stdout);
							continue;
						}

					}
					if(try == 4) {

						printf("Too many failed attempts! Returning to main screen...\n");
						fflush(stdout);

						echo_on();

						return authenticated;
					}

					break;
				}

			}
			if(!authenticated) {

				printf("User Name does not exist!\n");
				fflush(stdout);

				echo_on();

				return authenticated;
			}

			break;
		}



		printf("Successfully Authenticated\n");
		fflush(stdout);

		echo_on();

		return authenticated;
	}

	int check_pass(char *pass_hash, char *salt, char *passphrase) {

		int match = 0;

		char *cpasshash;

		cpasshash = (char *)malloc(65);

		sha256_string(passphrase, cpasshash, salt);

		if(!(strcmp(cpasshash, pass_hash)))
			match = 1;

		free(cpasshash);

		return match;
	}

	void Create_User() {

			FILE *udf, *bf; // User Data File
			int aexists;

			char badchars[8];

			sprintf(badchars, "\\\"\':;. ");

			char user[25], *fuser, buf[123], pass[49], passcheck[49], passhash[65], salt[33];

			int try = 0;

			printf("\n");
			fflush(stdout);

			while(1) {

				try++;

				if((udf = fopen("Users.dat", "r+")))
					break;

				if(try > 2) {

					printf("User data file not found or corrupted!\n");
					fflush(stdout);
					break;
				}
			}

			try = 0;

			if(!udf) {

				printf("Attempting to recover from backup...\n");
				fflush(stdout);

				while(1) {

					try++;

					if((bf = fopen("Users.bak", "r")))
						break;

					if(try > 2) {

						printf("User data backup not found or corrupted! Creating new files...\n");
						printf("If this is the first time you are using the app, this behaviour is normal\n");
						fflush(stdout);

						udf = fopen("Users.dat", "w");
						bf = fopen("Users.bak", "w");
						fclose(udf);
						fclose(bf);
						udf = fopen("Users.dat", "r+");

						break;

					}

				}

				if(!udf) {

					File_Copy("Users.bak", "Users.dat");

					printf("Recovery Successful!\n");
					fflush(stdout);
				}
				udf = fopen("Users.dat", "r+");
			}

			printf("User Name must be max 24 characters long, unique, and must not contain \\, \", \', :, ;, . or spaces\n");
			printf("Warning: If the User Name is longer than 24 characters, only the first 24 will be taken into consideration\n");
			fflush(stdout);

			do {

				while(1) {

					printf("Enter a User Name: ");
					fflush(stdout);

					fgets(user, 25, stdin);
					user[strlen(user) - 1] = '\0';

					int	sc = 0;
					for(int i = 0; i < 7; i++) {

						if(strchr(user, badchars[i])) {

							printf("\nUser Name contains bad characters!!\n");
							fflush(stdout);
							sc = 1;
						}
					}
					if(sc)
						continue;

					break;
				}

				aexists = 0;

				while(!feof(udf)) {

					fscanf(udf, "%s", buf);
					fuser = strtok(buf, ":");

					if(!(strcmp(user, fuser))) {

						aexists = 1;
						printf("\nUser Name already exists!\n");
						fflush(stdout);
						fseek(udf, 0, SEEK_SET);
						break;
					}
				}

			}while(aexists);

			fprintf(udf, "%s", user);

			do {
				aexists = 1; // Using the same variable for memory efficiency
				printf("\nPass Phrase must be at least 16 characters and at most 48 characters long and it must not contain bad characters (\\, \", \', ., :, ;)\n");
				printf("Warning if the Pass Phrase is longer than 48 characters only the first 48 will be taken into consideration\n");
				fflush(stdout);

				echo_off();

				while(1) {

					printf("Enter a Pass Phrase: ");
					fflush(stdout);

					fgets(pass, 49, stdin);
					pass[strlen(pass) - 1] = '\0';

					if(strlen(pass) < 16) {

						printf("Password has less than 16 characters!\n");
						fflush(stdout);
						continue;
					}

					int	sc = 0;
					for(int i = 0; i < 6; i++) {

						if(strchr(pass, badchars[i])) {

							printf("\nPass Phrase contains bad characters!!\n");
							fflush(stdout);
							sc = 1;
						}
					}

					if(sc)
						continue;

					break;
				}

				printf("\nPlease enter your password again: ");
				fflush(stdout);

				fgets(passcheck, 49, stdin);
				passcheck[strlen(passcheck) - 1] = '\0';

				aexists = strcmp(pass, passcheck);

				if(aexists) {

					printf("\nPasswords do not match!\n");
					fflush(stdout);
				}

			}while(aexists);

			echo_on();

			fprintf(udf, ":");

			salt_generator(salt);

			sha256_string(pass, passhash, salt);

			fprintf(udf, "%s.%s\n", passhash, salt);

			printf("User successfully created!");

			fseek(udf, 0, SEEK_SET);

			fclose(udf);

			File_Copy("Users.dat", "Users.bak");
		}

	void Add_Entry(user_data udat) {

		FILE *ef, *df, *bf;

		char service[50];
		char password[50];
		char buf[102];
		char *fservice;

		int try = 0, sc = 0;

		printf("\n");
		fflush(stdout);

		if(!(Dir_Check("User_Data")))
			mkdir("User_Data");

		while(1) {

			try ++;

			if((ef = fopen(udat.fname, "rb")))
				break;

			if(try > 2) {

				printf("User File not found or corrupted!\n");
				fflush(stdout);
				break;
			}
		}

		if(ef == NULL) {

			printf("Attempting recovery from backup!\n");
			fflush(stdout);

			try = 0;

			while(1) {

				try++;

				if((bf = fopen(udat.bfname, "r"))) {

					fclose(bf);

					File_Copy(udat.bfname, udat.fname);

					printf("Recovery Successful!\n");
					fflush(stdout);

					ef = fopen(udat.fname, "rb");
					df = fopen(udat.dfname, "wb");

					en_de_crypt(FALSE, ef, df, udat.ckey, udat.iv);
					fclose(df);
					fclose(ef);

					df = fopen(udat.dfname, "r");

					break;
				}
				if(try > 2) {

					printf("Backup file not found or corrupted!\n");
					printf("Creating new files...\n");
					printf("If this is the first time you are using the app this behaviour is normal\n");
					fflush(stdout);

					bf = fopen(udat.bfname, "w");
					ef = fopen(udat.fname, "w");
					df = fopen(udat.dfname, "w");

					fclose(bf);
					fclose(ef);
					fclose(df);

					df = fopen(udat.dfname, "r");

					fseek(df, 0, SEEK_END);

					break;
				}
			}
		}
		else {

			df = fopen(udat.dfname, "wb");

			en_de_crypt(FALSE, ef, df, udat.ckey, udat.iv);

			fclose(ef);
			fclose(df);

			df = fopen(udat.dfname, "r");
		}

		while(1) {

			printf("Enter the service/username/account_name associated with the password It cannot contain \':\', and can have a maximum of 50 characters: ");
			fflush(stdout);

			fgets(service, 50, stdin);
			service[strlen(service)-1] = '\0';

			if((strchr(service, ':'))) {

				printf("Service contains \':\' !\n");
				fflush(stdout);

				continue;
			}

			while(!feof(df)) {

				fgets(buf, 101, df);

				fservice = strtok(buf, ":");

				sc = 0;

				if(!(strcmp(service, fservice))) {

					printf("Service already exists!\n");
					fflush(stdout);
					fseek(df, 0, SEEK_SET);

					sc = 1;
					break;
				}
			}

			if(sc)
				continue;

			break;
		}

		fclose(df);

		df = fopen(udat.dfname, "a");

		printf("\nEnter the password you wish to save (max 50 characters): ");
		fflush(stdout);

		fgets(password, 50, stdin);
		password[strlen(password)-1] = '\0';

		fprintf(df, "%s:%s\n", service, password);

		printf("\nPassword added successfully!\n");

		fclose(df);

		df = fopen(udat.dfname, "rb");
		ef = fopen(udat.fname, "wb");

		en_de_crypt(TRUE, df, ef, udat.ckey, udat.iv);

		fclose(df);
		fclose(ef);

		remove(udat.dfname);

		File_Copy(udat.fname, udat.bfname);

	}

void Ret_All(user_data udat) {

	FILE *ef, *df, *bf;

	char *service;
	char *password;
	char buf[102];

	int try = 0;

	printf("\n");
	fflush(stdout);

	if(!(Dir_Check("User_Data"))) {

		printf("Directory containing user data not found. You first need to add passwords before you can retrieve any\n");
		fflush(stdout);

		return;
	}

	while(1) {

		try ++;

		if((ef = fopen(udat.fname, "rb")))
			break;

		if(try > 2) {

			printf("User File not found or corrupted!\n");
			fflush(stdout);
			break;
		}
	}

	if(ef == NULL) {

		printf("Attempting recovery from backup!\n");
		fflush(stdout);

		try = 0;

		while(1) {

			try++;

			if((bf = fopen(udat.bfname, "r"))) {

				fclose(bf);

				File_Copy(udat.bfname, udat.fname);

				ef = fopen(udat.fname, "rb");
				df = fopen(udat.dfname, "wb");

				printf("Recovery Successful!\n");
				fflush(stdout);

				en_de_crypt(FALSE, ef, df, udat.ckey, udat.iv);
				fclose(df);
				fclose(ef);

				df = fopen(udat.dfname, "r");

				break;
			}
			if(try > 2) {

				printf("Backup file not found or corrupted!\n");
				printf("If this is the first time you are using the app, then you first need to add at least a password\nbefore you can retrieve any\n");
				fflush(stdout);

				return;
			}
		}
	}
	else {

		df = fopen(udat.dfname, "wb");

		en_de_crypt(FALSE, ef, df, udat.ckey, udat.iv);

		fclose(ef);
		fclose(df);

		df = fopen(udat.dfname, "r");
	}

	printf("Retrieving Requested Information...\n\n");
	fflush(stdout);

		while(!feof(df)) {

			fgets(buf, 101, df);
			service = strtok(buf, ":");
			password = strtok(NULL, "\n");

			if((service) && (password)) {
				printf("%s: %s\n", service, password);
				fflush(stdout);
			}
		}

		fclose(df);
		remove(udat.dfname);
}

	void Ret_Spec(user_data udat) {

		FILE *ef, *df, *bf;

		char buf[102];
		char service[50];
		char *password;
		char *fservice;

		int shown = 0;

		int try = 0;

		printf("\n");
		fflush(stdout);

		if(!(Dir_Check("User_Data"))) {

			printf("Directory containing user data not found. You first need to add passwords before you can retrieve any\n");
			fflush(stdout);

			return;
		}

		while(1) {

		try ++;

		if((ef = fopen(udat.fname, "rb")))
			break;

		if(try > 2) {

			printf("User File not found or corrupted!\n");
			fflush(stdout);
			break;
		}
	}

	if(ef == NULL) {

		printf("Attempting recovery from backup!\n");
		fflush(stdout);

		try = 0;

		while(1) {

			try++;

			if((bf = fopen(udat.bfname, "r"))) {

				fclose(bf);

				File_Copy(udat.bfname, udat.fname);

				ef = fopen(udat.fname, "rb");
				df = fopen(udat.dfname, "wb");

				en_de_crypt(FALSE, ef, df, udat.ckey, udat.iv);
				fclose(df);
				fclose(ef);

				printf("Recovery Successful!\n");
				fflush(stdout);

				df = fopen(udat.dfname, "r");

				break;
			}
			if(try > 2) {

				printf("Backup file not found or corrupted!\n");
				printf("If this is the first time you are using the app, then you first need to add at least a password\nbefore you can retrieve any\n");
				fflush(stdout);

				return;
			}
		}
	}
	else {

		df = fopen(udat.dfname, "wb");

		en_de_crypt(FALSE, ef, df, udat.ckey, udat.iv);

		fclose(ef);
		fclose(df);

		df = fopen(udat.dfname, "r");
	}

		printf("Enter the service you wish to retrieve the passwords for: ");
		fflush(stdout);

		fgets(service, 50, stdin);
		service[strlen(service)-1] = '\0';

		printf("Retrieving requested information...\n\n");
		fflush(stdout);

		while(!feof(df)) {

			fgets(buf, 101, df);

			fservice = strtok(buf, ":");

			if(!(strcmp(service, fservice))) {

				password = strtok(NULL, "\n");

				if((password)) {
					printf("%s: %s\n", service, password);
					fflush(stdout);
					shown++;
				}
			}
		}

		if(!shown) {

			printf("No passwords were found for service: %s", service);
			fflush(stdout);
		}

		fclose(df);
		remove(udat.dfname);

	}


	void Show_Graph(user_data udat) {

		int try = 0, i;

		n = -1;

		FILE *ef, *df, *bf;

		char buf[102], *password;

		if(!(Dir_Check("User_Data"))) {

			printf("\nDirectory containing user data not found. You first need to add passwords before you can render a graph\n");
			fflush(stdout);

			return;
		}

		while(1) {
			try ++;

			if((ef = fopen(udat.fname, "rb")))
				break;

			if(try > 2) {

				printf("User File not found or corrupted!\n");
				fflush(stdout);
				break;
			}
		}

		if(ef == NULL) {

			printf("Attempting recovery from backup!\n");
			fflush(stdout);

			try = 0;

			while(1) {

				try++;

				if((bf = fopen(udat.bfname, "r"))) {

					fclose(bf);

					File_Copy(udat.bfname, udat.fname);

					ef = fopen(udat.fname, "rb");
					df = fopen(udat.dfname, "wb");

					en_de_crypt(FALSE, ef, df, udat.ckey, udat.iv);
					fclose(df);
					fclose(ef);

					printf("Recovery Successful!\n");
					fflush(stdout);

					df = fopen(udat.dfname, "r");

					break;
				}
				if(try > 2) {

					printf("Backup file not found or corrupted!\n");
					printf("If this is the first time you are using the app, then you first need to add at least a password\nbefore you see a graph showing your usage of services\n");
					fflush(stdout);

					return;
				}
			}
		}
		else {

			df = fopen(udat.dfname, "wb");

			en_de_crypt(FALSE, ef, df, udat.ckey, udat.iv);

			fclose(ef);
			fclose(df);

			df = fopen(udat.dfname, "r");
		}


		while(!feof(df)) {

			fgets(buf, 101, df);
			n++;
		}

		rewind(df);

		v_arr = (int *)malloc(sizeof(int) * n);

		for(i = 0; i < n; i ++) {

			fgets(buf, 101, df);
			strtok(buf, ":");
			password = strtok(NULL, "\n");

			if(password)
				v_arr[i] = strlen(password);

		}

		fclose(df);

		gl_arr = (float *)malloc(sizeof(float)*n);
		color_arr = (float *)malloc(sizeof(float)*n);

		for(i = 0; i < n; i++)
			color_arr[i] = gl_arr[i] = v_arr[i];


		int min = gl_arr[0], max = gl_arr[0];

		for(i = 1; i < n; i++) {

			if(gl_arr[i] > max)
				max = gl_arr[i];

			if(gl_arr[i] < min)
				min = gl_arr[i];
		}

		// Normalizing array to a range of .1 to .8

		for(i = 0; i < n; i++)
			color_arr[i] = gl_arr[i] = (gl_arr[i] - min) / (max - min);


		for(i = 0; i < n; i++) {

			gl_arr[i] = gl_arr[i] * 0.7 + 0.1;
			color_arr[i] = color_arr[i] * 0.85 + 0.1;
		}

		remove(udat.dfname);


		int argc;

		char **argv;

		glutInit(&argc, argv);
		glutInitWindowSize(1360, 765); //16:9 aspect ratio
		glutInitWindowPosition(280, 157); //Center of screen, determined relative to window size
		glutCreateWindow("Password Length Graph");
		glutDisplayFunc(Display);
		glutMainLoop();

		free(gl_arr);
		free(v_arr);

		return;
	}

	void Del_Entry(user_data udat) {

		FILE *ef, *df, *bf, *tmp;

		char service[50];
		char *fservice;
		char buf[102];
		char tokstr[102];

		int try = 0;

		char tmpfname[17];

		sprintf(tmpfname, ".\\User_Data\\temp");

		printf("\n");
		fflush(stdout);

		if(!(Dir_Check("User_Data"))) {

			printf("Directory containing user data not found. You first need to add passwords before you can delete any\n");
			fflush(stdout);

			return;
		}

		while(1) {

			try ++;

			if((ef = fopen(udat.fname, "rb")))
				break;

			if(try > 2) {

				printf("User File not found or corrupted!\n");
				fflush(stdout);
				break;
			}
		}

		if(ef == NULL) {

			printf("Attempting recovery from backup!\n");
			fflush(stdout);

			try = 0;

			while(1) {

				try++;

				if((bf = fopen(udat.bfname, "r"))) {

					fclose(bf);

					File_Copy(udat.bfname, udat.fname);

					ef = fopen(udat.fname, "rb");
					df = fopen(udat.dfname, "wb");

					printf("Recovery Successful!\n");
					fflush(stdout);

					en_de_crypt(FALSE, ef, df, udat.ckey, udat.iv);
					fclose(df);
					fclose(ef);

					df = fopen(udat.dfname, "r");

					break;
				}
				if(try > 2) {

					printf("Backup file not found or corrupted!\n");
					printf("If this is the first time you are using the app, then you first need to add at least a password\nbefore you can retrieve any\n");
					fflush(stdout);

					return;
				}
			}
		}
		else {

			df = fopen(udat.dfname, "wb");

			en_de_crypt(FALSE, ef, df, udat.ckey, udat.iv);

			fclose(ef);
			fclose(df);

			df = fopen(udat.dfname, "r");
		}

		printf("Enter the service for which you would like to delete the saved password: ");
		fflush(stdout);

		fgets(service, 50, stdin);
		service[strlen(service) - 1] = '\0';

		printf("\nDeleting...\n");
		fflush(stdout);


		tmp = fopen(tmpfname, "w");

		while(!feof(df)) {

			fgets(buf, 101, df);

			printf("%s\n", buf);

			strcpy(tokstr, buf);

			fservice = strtok(tokstr, ":");

			if(!feof(df) && strcmp(service, fservice))
				fprintf(tmp, "%s", buf);
		}

		fclose(df);
		fclose(tmp);

		File_Copy(tmpfname, udat.dfname);

		df = fopen(udat.dfname, "rb");
		ef = fopen(udat.fname, "wb");

		en_de_crypt(TRUE, df, ef, udat.ckey, udat.iv);

		fclose(df);
		fclose(ef);

		remove(udat.dfname);
		remove(tmpfname);

		File_Copy(udat.fname, udat.bfname);

		printf("Successfully Deleted\n");
		fflush(stdout);

	}

	void Modify_Entry(user_data udat) {

		FILE *ef, *df, *bf, *tmp;

		char service[50];
		char nservice[50];
		char password[50];
		char *fservice;
		char buf[102];
		char tokstr[102];

		int try = 0, sc = 0;

		char tmpfname[17];

		sprintf(tmpfname, ".\\User_Data\\temp");

		printf("\n");
		fflush(stdout);

		if(!(Dir_Check("User_Data"))) {

			printf("Directory containing user data not found. You first need to add passwords before you can modify any\n");
			fflush(stdout);

			return;
		}

		while(1) {

			try ++;

			if((ef = fopen(udat.fname, "rb")))
				break;

			if(try > 2) {

				printf("User File not found or corrupted!\n");
				fflush(stdout);
				break;
			}
		}

		if(ef == NULL) {

			printf("Attempting recovery from backup!\n");
			fflush(stdout);

			try = 0;

			while(1) {

				try++;

				if((bf = fopen(udat.bfname, "r"))) {

					fclose(bf);

					File_Copy(udat.bfname, udat.fname);

					ef = fopen(udat.fname, "rb");
					df = fopen(udat.dfname, "wb");

					printf("Recovery Successful!\n");
					fflush(stdout);

					en_de_crypt(FALSE, ef, df, udat.ckey, udat.iv);
					fclose(df);
					fclose(ef);

					df = fopen(udat.dfname, "r");

					break;
				}
				if(try > 2) {

					printf("Backup file not found or corrupted!\n");
					printf("If this is the first time you are using the app, then you first need to add at least a password\nbefore you can retrieve any\n");
					fflush(stdout);

					return;
				}
			}
		}
		else {

			df = fopen(udat.dfname, "wb");

			en_de_crypt(FALSE, ef, df, udat.ckey, udat.iv);

			fclose(ef);
			fclose(df);

			df = fopen(udat.dfname, "r");
		}

		printf("What service would you like to modify ?\n");
		fflush(stdout);

		fgets(service, 50, stdin);
		service[strlen(service) - 1] = '\0';

		while(1) {
			printf("Enter the new service/username/account_name associated with the password It cannot contain \':\', and can have a maximum of 50 characters: ");
			fflush(stdout);

			fgets(nservice, 50, stdin);
			nservice[strlen(nservice)-1] = '\0';

			if((strchr(nservice, ':'))) {

				printf("Service contains \':\' !\n");
				fflush(stdout);

				continue;
			}

			sc = 0;

			if(strcmp(service, nservice)) {

				while(!feof(df)) {

					fgets(buf, 101, df);

					fservice = strtok(buf, ":");

					if(!(strcmp(nservice, fservice))) {

						printf("Service already exists!\n");
						fflush(stdout);
						fseek(df, 0, SEEK_SET);

						sc = 1;
						break;
					}
				}
			}

			if(sc)
				continue;

			break;
		}

		printf("\nEnter the password you wish to save (max 50 characters): ");
		fflush(stdout);

		fgets(password, 50, stdin);
		password[strlen(password)-1] = '\0';

		tmp = fopen(tmpfname, "w");

		rewind(df);

		while(!feof(df)) {

			fgets(buf, 101, df);

			strcpy(tokstr, buf);

			fservice = strtok(tokstr, ":");

			if(!(strcmp(service, fservice)))
				fprintf(tmp, "%s:%s\n", nservice, password);
			else
				if(!feof(df))
					fprintf(tmp, "%s\n", buf);

		}

		fclose(df);
		fclose(tmp);

		File_Copy(tmpfname, udat.dfname);

		df = fopen(udat.dfname, "rb");
		ef = fopen(udat.fname, "wb");

		en_de_crypt(TRUE, df, ef, udat.ckey, udat.iv);

		fclose(df);
		fclose(ef);

		remove(udat.dfname);
		remove(tmpfname);

		File_Copy(udat.fname, udat.bfname);

		printf("Successfully Modified!\n");
		fflush(stdout);

	}

	int main() {

		int auth = 0;

		user_data udat;

		while(!auth) {
			auth = CA_Screen(&udat);

			if(auth == -1)
				return 0;

			for(int i = 0; i < 16; i+=2) {

				udat.iv[i] = udat.ckey[i+1];
				udat.iv[i+1] = udat.ckey[i];
			}

			udat.iv[16] = '\0';

			strncpy(udat.dfname, udat.fname, 12);
			strcat(udat.dfname, "d");
			strcat(udat.dfname, udat.fname+12);

			Auth_Screen(&auth, &udat);
		}

		return 0;
	}
