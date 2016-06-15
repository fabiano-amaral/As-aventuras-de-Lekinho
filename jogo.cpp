#include "Obstaculo.h"
#include "Personagem.h"
#include "Fase.h"

string str="000000010";
float fatorVelocidade = 0.5;

Personagem *lekinho;
Obstaculo *obstaculo1;
Obstaculo *obstaculo2;
Fase *fase;

void init() {
        
	srand(time(NULL));
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0,WIDTH,0,HEIGHT,-100,100);
    
	lekinho = new Personagem (LEKINHO, PISTA2, 50, 0.06*HEIGHT, true);
	fase = new Fase (FLORESTA); //TESTE INICIANDO COM FLORESTA
	obstaculo1 = fase->getObstaculo();
	fase->ocupaPista(obstaculo1->getX(), true);
	obstaculo2 = fase->obstaculoSegundoProbabilidade(0.3);
	if (obstaculo2) {
			float pistaObstaculo2;
			do pistaObstaculo2 = PISTA_ALEATORIA; while (pistaObstaculo2 == obstaculo1->getX());
			obstaculo2->setX(pistaObstaculo2);
			fase->ocupaPista(obstaculo2->getX(), true);
	}
}

void print(int x, int y, string& string){
	glPushMatrix();
		glColor3f(1,1,0);
		glRasterPos2f(x,y);
		int len = (int) string.size();
		for (int i = 0; i < len; i++){
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,string[i]);
		}
	glPopMatrix();
}

bool colide (Elemento* elemento1, Elemento* elemento2) {
	return 	elemento1 != NULL &&
			elemento2 != NULL &&
			elemento1->podeColidir() &&
			elemento2->podeColidir() &&
			elemento2->yInferior() < elemento1->ySuperior() &&
			elemento2->ySuperior() > elemento1->yInferior() &&
			elemento1->getX() == elemento2->getX();
}

void atualizaObstaculos () {
	obstaculo1->setY(obstaculo1->getY()-(8*fatorVelocidade));
	obstaculo1->realizaAcao(fase->getPistasLivres());
	obstaculo1->setCiclo(obstaculo1->getCiclo() + 1);	
	if (obstaculo2) {
		obstaculo2->setY(obstaculo2->getY()-(8*fatorVelocidade));
		obstaculo2->realizaAcao(fase->getPistasLivres());
		obstaculo2->setCiclo(obstaculo2->getCiclo() + 1);
	}

	if (obstaculo1->ySuperior() < 0){
		fase->ocupaPista(obstaculo1->getX(), false);
		if (obstaculo2)
			fase->ocupaPista(obstaculo2->getX(), false);
		
		obstaculo1 = fase->getObstaculo();
		obstaculo2 = fase->obstaculoSegundoProbabilidade(0.3);

		obstaculo1->setX(PISTA_ALEATORIA);
		obstaculo1->setY(HEIGHT + 100);
		obstaculo1->setCiclo(0);	
		fase->ocupaPista(obstaculo1->getX(), true);
		if (obstaculo2) {
			float pistaObstaculo2;
			do pistaObstaculo2 = PISTA_ALEATORIA; while (pistaObstaculo2 == obstaculo1->getX());
			obstaculo2->setX(pistaObstaculo2);
			obstaculo2->setY(HEIGHT + 100);
			obstaculo2->setCiclo(0);
			fase->ocupaPista(obstaculo2->getX(), true);
		} 
	}
}	

void idle(){
	/*stringstream sstr;
	int a;
	sstr.clear();
	sstr << str;
	sstr >> a;
	a++;
	sstr << a;
	str.clear();
	str = sstr.str();*/
	atualizaObstaculos();
	if (colide(lekinho, obstaculo1) || colide(lekinho, obstaculo2))
		exit(0);
	fatorVelocidade+=0.0000001;
	glutPostRedisplay();
}

void keyboard(unsigned char tecla, int x, int y){
	switch(tecla){
		case 'd':
			if(lekinho->getX() < PISTA3)
				lekinho->setX(lekinho->getX() + WIDTH/3.0);
			break;
		case 'a':
			if(lekinho->getX() > PISTA1)
				lekinho->setX(lekinho->getX() - WIDTH/3.0);
			break;
			
		//TESTE - APAGAR: apertar 1 para fase de floresta, 2 para fase de gelo e 3 para fase de deserto
		case '1':
			fase = new Fase (FLORESTA);
			break;
		case '2':
			fase = new Fase (GELO);
			break;
		case '3':
			fase = new Fase (DESERTO);
			break;
		//
	}
	glutPostRedisplay();
}

void display(){
		glClear(GL_COLOR_BUFFER_BIT);

		glPushMatrix();
			print(490,380,str);
		glPopMatrix();

		obstaculo1->desenha(obstaculo1->getX(), obstaculo1->getY(), obstaculo1->getCiclo());
		if (obstaculo2)
			obstaculo2->desenha(obstaculo2->getX(), obstaculo2->getY(), obstaculo2->getCiclo());
		lekinho->desenha(lekinho->getX(), lekinho->getY(), lekinho->getCiclo());

		glutSwapBuffers();
}

int main(int argc,char** argv){
		glutInit(&argc,argv);
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
		glutInitWindowPosition(250,200);
		glutInitWindowSize(WIDTH,HEIGHT);
		glutCreateWindow("As aventuras de Lekinho");
		init();
		glutDisplayFunc(display);
		glutKeyboardFunc(keyboard);
		glutIdleFunc(idle);
		glutMainLoop();
}
