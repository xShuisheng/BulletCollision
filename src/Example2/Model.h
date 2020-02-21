#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "Shader.h"
#include "DefaultRenderObject.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
using namespace std;



class Model
{
public:
	/*  Model Data */
	//vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
	vector<DefaultRenderObject> meshes;
	string directory;
	bool gammaCorrection;

	Shader shader;

private:
	map<string, unsigned int> _textureMap;

public:

	/*  Functions   */
	// constructor, expects a filepath to a 3D model.
	Model(string const &path, bool gamma = false) : gammaCorrection(gamma),shader("simple.vs", "simple.fs")
	{
		loadModel(path);
	}

	// draws the model, and thus all its meshes
	void Render(glm::mat4 tran)
	{
		for (unsigned int i = 0; i < meshes.size(); i++)
		{
			meshes[i].render(tran);
		}
	}

	vector<DefaultRenderObject> & getObjects()
	{
		return meshes;
	}

private:
	/*  Functions   */
	// loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
	void loadModel(string const &path)
	{
		// read file via ASSIMP
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
		// check for errors
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
		{
			cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
			return;
		}
		// retrieve the directory path of the filepath
		directory = path.substr(0, path.find_last_of('/'));

		// process ASSIMP's root node recursively
		processNode(scene->mRootNode, scene);
	}

	// processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
	void processNode(aiNode *node, const aiScene *scene)
	{
		// process each mesh located at the current node
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			// the node object only contains indices to index the actual objects in the scene. 
			// the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			processMesh(mesh, scene);
		}
		// after we've processed all of the meshes (if any) we then recursively process each of the children nodes
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene);
		}

	}

	void processMesh(aiMesh *mesh, const aiScene *scene)
	{
		// data to fill
		//vector<Vertex> vertices;
		//vector<unsigned int> indices;
		//vector<Texture> textures;

		meshes.push_back(DefaultRenderObject());
		DefaultRenderObject & curObject= meshes[meshes.size() - 1];
		curObject.pshader = &shader;
		curObject.useTexture(false);

		unsigned int * pindices = new unsigned int[3 * mesh->mNumFaces];
		float * pvertices = new float[3 * mesh->mNumVertices];
		float * pnormals = new float[3 * mesh->mNumVertices];
		float * ptexcoords = 0;
		if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
		{
			//ptexcoords = new float[2 * mesh->mNumVertices];
		}

		// Walk through each of the mesh's vertices
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{							  
			// positions
			pvertices[3 * i] = mesh->mVertices[i].x;
			pvertices[3 * i + 1] = mesh->mVertices[i].y;
			pvertices[3 * i + 2] = mesh->mVertices[i].z;

			// normals
			if (mesh->HasNormals())
			{
				pnormals[3 * i] = mesh->mNormals[i].x;
				pnormals[3 * i + 1] = mesh->mNormals[i].y;
				pnormals[3 * i + 2] = mesh->mNormals[i].z;
			}
		
			
			//// texture coordinates
			//if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
			//{
			//	// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
			//	// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
			//	ptexcoords[2 * i] = mesh->mTextureCoords[0][i].x;
			//	ptexcoords[2 * i + 1] = mesh->mTextureCoords[0][i].y;
			//	
			//}
		}

		// now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
		int tmp_idx = 0;
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			// retrieve all indices of the face and store them in the indices vector
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				pindices[tmp_idx++] = (face.mIndices[j]);
		}

		_generateNormals(mesh->mNumVertices, pvertices, mesh->mNumFaces, pindices, pnormals);

		/*
		// process materials
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		// we assume a convention for sampler names in the shaders. Each diffuse texture should be named
		// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
		// Same applies to other texture as the following list summarizes:
		// diffuse: texture_diffuseN
		// specular: texture_specularN
		// normal: texture_normalN

		// 1. diffuse maps
		//vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		
		if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
		{
			aiString str;
			material->GetTexture(aiTextureType_DIFFUSE, 0, &str);

			string texturePath = str.C_Str();
			unsigned int textureId = 0;

			auto iter = _textureMap.find(texturePath);
			if (iter == _textureMap.end())
			{
				curObject.getMaterial().diffuse.loadTexture((directory + texturePath).c_str());
			}
			else
			{
				curObject.getMaterial().diffuse.id_texture = _textureMap[texturePath];
			}
		}
		*/

		curObject.setVertexes(mesh->mNumVertices, pvertices);
		curObject.setNormal(pnormals);
		//curObject.setTexcoord(ptexcoords);
		curObject.setIndexes(mesh->mNumFaces, pindices);

		curObject.bindAll();
	
	}

	glm::vec3 _cross(const glm::vec3& v1, const glm::vec3& v2)
	{
		return glm::vec3(
			v1.y*v2.z - v1.z*v2.y,
			v1.z*v2.x - v1.x*v2.z,
			v1.x*v2.y - v1.y*v2.x
		);
	}


	void _generateNormals(int num_vertices, float* pvertices, int num_faces, unsigned int* pindices, float* pnormals)
	{
		vector<int> neigh_count(num_vertices, 0);

		for (int i = 0; i < (3 * num_vertices); ++i)
		{
			pnormals[i] = 0.0;
		}

		for (int i = 0; i < num_faces; ++i)
		{
			int idx0 = pindices[3 * i];
			int idx1 = pindices[3 * i + 1];
			int idx2 = pindices[3 * i + 2];

			glm::vec3 p0(pvertices[3 * idx0], pvertices[3 * idx0 + 1], pvertices[3 * idx0 + 2]);
			glm::vec3 p1(pvertices[3 * idx1], pvertices[3 * idx1 + 1], pvertices[3 * idx1 + 2]);
			glm::vec3 p2(pvertices[3 * idx2], pvertices[3 * idx2 + 1], pvertices[3 * idx2 + 2]);

			glm::vec3 nor = _cross((p1 - p0), (p2 - p0));
			float len = nor.length();
			nor = glm::vec3(nor.x / len, nor.y / len, nor.z / len);

			float * n = pnormals + 3 * idx0;
			n[0] += nor.x;	n[1] += nor.y;	n[2] += nor.z;
			neigh_count[idx0]++;

			n = pnormals + 3 * idx1;
			n[0] += nor.x;	n[1] += nor.y;	n[2] += nor.z;
			neigh_count[idx1]++;

			n = pnormals + 3 * idx2;
			n[0] += nor.x;	n[1] += nor.y;	n[2] += nor.z;
			neigh_count[idx2]++;
		}

		for (int i = 0; i < num_vertices; ++i)
		{
			float * nor = pnormals + 3 * i;
			float len = glm::vec3(nor[0], nor[1], nor[2]).length();

			nor[0] /= len;
			nor[1] /= len;
			nor[2] /= len;

			nor[0] =0.0;
			nor[1] =1.0;
			nor[2] =0.0;

		}
	}
};


#endif