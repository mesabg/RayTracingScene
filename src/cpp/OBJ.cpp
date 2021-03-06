#include <OBJ.h>

OBJ::OBJ(Routes* routes):Model(routes) {
	//PONER NOMBRE DEL ARCHIVO | RUTA
	// string archivo=openfilename(); //PILA
	string archivo = routes->model;
	if (archivo.empty()) return;
	ifstream fe(archivo);
	GLfloat* vertex;
	string::size_type sz;
	//char aux;
	bool sigoLeyendo = false;
	string leer;
	double x, y, z;
	this->index = 0;

	while (fe >> leer) {
		if (leer == "#" || leer[0] == '#') { //Saltar linea
			while (fe.get() != '\n') continue;
		}
		else if (leer == "v") {
		label3: fe >> leer;
			x = stod(leer, &sz);
			fe >> leer;
			y = stod(leer, &sz);
			fe >> leer;
			z = stod(leer, &sz);
			vertex = new GLfloat[3];
			vertex[0] = (GLfloat)x; vertex[1] = (GLfloat)y; vertex[2] = (GLfloat)z;

			if ((int)this->vertexes.size() == 0) {
				//this->boundingBox->initVertexBox(vertex);
				this->max = vertex[0];
			}
			this->vertexes.push_back(vertex);
			////this->boundingBox->setVertexBox(vertex);

			if (vertex[0] >= this->max) this->max = vertex[0];
			if (vertex[1] >= this->max) this->max = vertex[1];
			if (vertex[2] >= this->max) this->max = vertex[2];
		}
		else if (leer == "vn") {
			fe >> leer;
			x = stod(leer, &sz);
			fe >> leer;
			y = stod(leer, &sz);
			fe >> leer;
			z = stod(leer, &sz);
			vertex = new GLfloat[3];
			vertex[0] = (GLfloat)x; vertex[1] = (GLfloat)y; vertex[2] = (GLfloat)z;
			this->normal.push_back(vertex);
		}
		else if (leer == "vt") {
			fe >> leer;
			x = stod(leer, &sz);
			fe >> leer;
			y = stod(leer, &sz);
			//fe >> leer;
			//z = stod(leer, &sz);
			//Cuando son solo 2 texturas
			z = 0.0f;
			vertex = new GLfloat[3];
			vertex[0] = (GLfloat)x; vertex[1] = (GLfloat)y; vertex[2] = (GLfloat)z;
			this->texture_.push_back(vertex);
		}
		else if (leer == "f") {
		label:
			vector<string> myarray;

			bool aceptar = true;
			while (fe >> leer) {
				//printf("%s\n",leer.c_str());
				//system("pause");
				if (leer == "f") {
					aceptar = true;
					break;
				}
				if (leer == "s" || leer == "#" || leer[0] == '#') {
				label2: while (fe.get() != '\n') continue;
					if (fe >> leer) {
						if (leer == "s" || leer == "#" || leer[0] == '#') {
							goto label2;
						}
						break;
					}
					else {
						return;
					}
				}
				aceptar = false;

				myarray.push_back(leer);

			}
			if (aceptar)
				agregarFace(myarray);

			if (leer == "f") {
				goto label;
			}
			else if (leer == "v") {
				goto label3;
			}

		}
	}
}

OBJ::~OBJ() {}


void OBJ::agregarFace(vector<string> myarray) {
	vector<int> vertices;
	vector<int> normals;
	vector<int> textures;


	for (int i = 0; i < (int)myarray.size(); i++) {
		//Vertices
		vector <string> elems;
		split(myarray[i], '/', elems);

		vertices.push_back(atoi(elems[0].c_str()) - 1);
		normals.push_back(atoi(elems[2].c_str()) - 1);
		textures.push_back(atoi(elems[1].c_str()) - 1);

		/*char aux = myarray[i][0];
		string Auxiliar = "";
		int j = 1;

		for (int j = 1; j <= (int)myarray[i].size() && aux != '/'; j++){
		Auxiliar.push_back(aux);
		if (j != (int)myarray[i].size())
		aux = (int)myarray[i][j];
		}
		vertices.push_back(atoi(Auxiliar.c_str())-1);
		//Normales
		j++;
		aux = myarray[i][j];
		Auxiliar = "";
		int k = 0;
		// 1/3/4  1/3/4  1/3/4 1/3/4  1/3/4
		//cout << j << endl;
		for (int k = j; k <= (int)myarray[i].size() && aux != '/'; k++){
		Auxiliar.push_back(aux);
		if (k != (int)myarray[i].size())
		aux = (int)myarray[i][k];
		}
		normals.push_back(atoi(Auxiliar.c_str())-1);
		//Texture
		k++;
		cout << k << endl;
		system("pause");
		aux = myarray[i][k];
		cout << aux << endl;
		Auxiliar = "";

		for (int m = k; m <= (int)myarray[i].size() && aux != '/'; m++){
		Auxiliar.push_back(aux);
		if (m != (int)myarray[i].size())
		aux = (int)myarray[i][m];
		}
		textures.push_back(atoi(Auxiliar.c_str())-1);*/
	}

	for (int i = 0; i < (int)vertices.size(); i++) {

		if (i == 0) {
			//this->glVBO = (GLfloat*)realloc(this->glVBO, (index+27) * sizeof(GLfloat));
			//cout << leer << endl;
			for (int j = 0; j < 3; j++) {
				this->glVBO.push_back(vertexes[vertices[j]][0]); index++;
				this->glVBO.push_back(vertexes[vertices[j]][1]); index++;
				this->glVBO.push_back(vertexes[vertices[j]][2]); index++;

				this->glVBO[index - 3] = this->glVBO[index - 3] / this->max;
				this->glVBO[index - 2] = this->glVBO[index - 2] / this->max;
				this->glVBO[index - 1] = this->glVBO[index - 1] / this->max;

				this->glVBO.push_back(normal[normals[j]][0]); index++;
				this->glVBO.push_back(normal[normals[j]][1]); index++;
				this->glVBO.push_back(normal[normals[j]][2]); index++;

				this->glVBO.push_back(texture_[textures[j]][0]); index++;
				this->glVBO.push_back(texture_[textures[j]][1]); index++;
				this->glVBO.push_back(texture_[textures[j]][2]); index++;

			}
			i = i + 2;
		}
		else {
			// :)
			//this->glVBO = (GLfloat*)realloc(this->glVBO, (index+27) * sizeof(GLfloat));

			this->glVBO.push_back(vertexes[vertices[0]][0]); index++;
			this->glVBO.push_back(vertexes[vertices[0]][1]); index++;
			this->glVBO.push_back(vertexes[vertices[0]][2]); index++;

			this->glVBO[index - 3] = this->glVBO[index - 3] / this->max;
			this->glVBO[index - 2] = this->glVBO[index - 2] / this->max;
			this->glVBO[index - 1] = this->glVBO[index - 1] / this->max;

			this->glVBO.push_back(normal[normals[0]][0]); index++;
			this->glVBO.push_back(normal[normals[0]][1]); index++;
			this->glVBO.push_back(normal[normals[0]][2]); index++;

			this->glVBO.push_back(texture_[textures[0]][0]); index++;
			this->glVBO.push_back(texture_[textures[0]][1]); index++;
			this->glVBO.push_back(texture_[textures[0]][2]); index++;

			this->glVBO.push_back(vertexes[vertices[i - 1]][0]); index++;
			this->glVBO.push_back(vertexes[vertices[i - 1]][1]); index++;
			this->glVBO.push_back(vertexes[vertices[i - 1]][2]); index++;

			this->glVBO[index - 3] = this->glVBO[index - 3] / this->max;
			this->glVBO[index - 2] = this->glVBO[index - 2] / this->max;
			this->glVBO[index - 1] = this->glVBO[index - 1] / this->max;

			this->glVBO.push_back(normal[normals[i - 1]][0]); index++;
			this->glVBO.push_back(normal[normals[i - 1]][1]); index++;
			this->glVBO.push_back(normal[normals[i - 1]][2]); index++;

			this->glVBO.push_back(texture_[textures[i - 1]][0]); index++;
			this->glVBO.push_back(texture_[textures[i - 1]][1]); index++;
			this->glVBO.push_back(texture_[textures[i - 1]][2]); index++;

			this->glVBO.push_back(vertexes[vertices[i]][0]); index++;
			this->glVBO.push_back(vertexes[vertices[i]][1]); index++;
			this->glVBO.push_back(vertexes[vertices[i]][2]); index++;

			this->glVBO[index - 3] = this->glVBO[index - 3] / this->max;
			this->glVBO[index - 2] = this->glVBO[index - 2] / this->max;
			this->glVBO[index - 1] = this->glVBO[index - 1] / this->max;

			this->glVBO.push_back(normal[normals[i]][0]); index++;
			this->glVBO.push_back(normal[normals[i]][1]); index++;
			this->glVBO.push_back(normal[normals[i]][2]); index++;

			this->glVBO.push_back(texture_[textures[i]][0]); index++;
			this->glVBO.push_back(texture_[textures[i]][1]); index++;
			this->glVBO.push_back(texture_[textures[i]][2]); index++;


			//this->faces.push_back(new Vertex(myarray[0], myarray[i - 1], myarray[i]));   // Si ocurre el caso en el q hay mas de 3, hay que triangularizar y meter otras 9 cosas
		}

	}
}