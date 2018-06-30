//
//  readObj.cpp
//  Face AR
//
//  Created by shiwei zhou on 2018/2/12.
//  Copyright © 2018年 shiwei zhou. All rights reserved.
//
#ifndef READ_OBJ_HPP_
#define READ_OBJ_HPP_


#include <cassert>
#include <fstream>
#include <experimental/optional>
#include <string>
#include <tuple>
#include <utility>
#include <vector>
#include <opencv2/core.hpp>
struct ObjMesh
{
    std::vector<cv::Point3f> vertices; ///< 3D vertex positions.
    std::vector<cv::Point3f> colors; ///< Colour information for each vertex. Expected to be in RGB order.
    std::vector<cv::Point2f> texcoords; ///< Texture coordinates for each vertex.
    
    std::vector<std::array<int, 3>> tvi; ///< Triangle vertex indices
    std::vector<std::array<int, 3>> tci; ///< Triangle color indices
};


template <class ContainerType>
void tokenize(const std::string& str, ContainerType& tokens, const std::string& delimiters = " ",
              bool trim_empty = false)
{
    std::string::size_type pos, last_pos = 0;
    const auto length = str.length();
    
    using value_type = typename ContainerType::value_type;
    using size_type = typename ContainerType::size_type;
    
    while (last_pos < length + 1)
    {
        pos = str.find_first_of(delimiters, last_pos);
        if (pos == std::string::npos)
        {
            pos = length;
        }
        
        if (pos != last_pos || !trim_empty)
            tokens.push_back(value_type(str.data() + last_pos, (size_type)pos - last_pos));
        
        last_pos = pos + 1;
    }
}

inline std::pair<cv::Point3f, cv::Point3f> parse_vertex(const std::string& line)
{
    std::vector<std::string> tokens;
    tokenize(line, tokens, " ");
    assert(tokens.size() == 3 || tokens.size() == 6); // Maybe we should throw instead?
    const cv::Point3f vertex(std::stof(tokens[0]), std::stof(tokens[1]), std::stof(tokens[2]));
    cv::Point3f vertex_color(-1,-1,-1);
    if (tokens.size() == 6)
    {
        vertex_color = cv::Point3f(std::stof(tokens[3]), std::stof(tokens[4]), std::stof(tokens[5]));
    }
    return {vertex, vertex_color};
}

inline cv::Point2f parse_texcoords(const std::string& line)
{
    std::vector<std::string> tokens;
    tokenize(line, tokens, " ");
  //  assert(tokens.size() == 2);
    const cv::Point2f texcoords(std::stof(tokens[0]), std::stof(tokens[1]));
    return texcoords;
}

inline void parse_vertex_normal(const std::string& line)
{
    throw std::runtime_error("Parsing \"vt\" is not yet implemented.");
}

// Note: Indices in obj's start at 1!
// Oh no, these contain 3 entries for triangles, but can be quads (=4 entries), and potentially more.
// Their triplets of: vertex, texture and normal indices. Some of them can be missing. E.g.:
//  f 1 2 3
//  f 3/1 4/2 5/3
//  f 6/4/1 3/5/3 7/6/5
//  f 7//1 8//2 9//3
inline auto parse_face(const std::string& line)
{
    using std::string;
    using std::vector;
    
    // Obj indices are 1-based.
    vector<int> vertex_indices;  // size() = 3 or 4
    vector<int> texture_indices; // size() = 3 or 4
    vector<int> normal_indices;  // size() = 3 or 4
    
    vector<string> tokens;
    vector<string> tokens_temp;
    tokenize(line, tokens, " ");
  //  assert(tokens.size() == 3 || tokens.size() == 4); // For now we need this to be 3 (triangles) or 4 (quads).
//    if(tokens_temp.size()>4){
//        for (int i=0; i<4; i++) {
//            tokens.push_back(tokens_temp[i]);
//        }
//    }
    // Now for each of these tokens, we want to split on "/":
    for (const auto& token : tokens)
    {
        vector<string> subtokens;
        tokenize(token, subtokens, "/"); // do we want trim_empty true or false?
        assert(subtokens.size() > 0 && subtokens.size() <= 3); // <= 3 correct or not?
        // Ok, let's make our life easy, for now only support the 1/2/3 syntax of the FaceWarehouse scans. In
        // fact the normal_indices are 0... we should check for that - zero-index = ignore, but note that, it's
        // probably a non-standard obj format extension.
      //  assert(subtokens.size() == 3);                         // FaceWarehouse
        vertex_indices.push_back(std::stoi(subtokens[0]) - 1); // obj indices are 1-based, so we subtract one.
        texture_indices.push_back(std::stoi(subtokens[1]) - 1);
        // subtokens[2] is zero, hence, no normal_indices.
    }
    
    return std::make_tuple(vertex_indices, texture_indices, normal_indices);
}


inline ObjMesh read_obj(std::string filename)
{
    std::ifstream file(filename);
    if (!file)
    {
        throw std::runtime_error(std::string("Could not open obj file: " + filename));
    }
    
    // We'll need these helper functions for the parsing:
    const auto starts_with = [](const std::string& input, const std::string& match) {
        return input.size() >= match.size() && std::equal(match.begin(), match.end(), input.begin());
    };
    
    /*    auto trim_left = [](const std::string& input, std::string pattern = " \t") {
     auto first = input.find_first_not_of(pattern);
     if (first == std::string::npos)
     {
     return input;
     }
     return input.substr(first, input.size());
     }; */
    
    ObjMesh objmesh;
    
    std::string line;
    while (getline(file, line))
    {
        if (starts_with(line, "#"))
        {
            continue;
        }
        
        if (starts_with(line, "v "))
        { // matching with a space so that it doesn't match 'vt'
            auto vertex_data =
            parse_vertex(line.substr(2)); // pass the string without the first two characters
            objmesh.vertices.push_back(
                                       cv::Point3f(vertex_data.first.x, vertex_data.first.y, vertex_data.first.z));
            // there are vertex colours:
            cv::Point3f a=vertex_data.second;
            if (a.x!=-1)
            {
                objmesh.colors.push_back(cv::Point3f(a.x,a.y,a.z));
            }
        }
        if (starts_with(line, "vt "))
        {
            const auto texcoords = parse_texcoords(line.substr(3));
            objmesh.texcoords.push_back(cv::Point2f(texcoords.x,texcoords.y));
        }
        if (starts_with(line, "vn "))
        {
            // detail::parse_vertex_normal(line.substr(3));
            // Not handled yet, our Mesh class doesn't contain normals right now anyway.
        }
        // There's other things like "vp ", which we don't handle
        if (starts_with(line, "f "))
        {
            auto face_data =parse_face(line.substr(2));
           
         //  cout<<std::get<1>(face_data)[0]<<"  "<<std::get<0>(face_data)[1]<<" "<<std::get<0>(face_data)[2]<<endl;
            if (std::get<0>(face_data).size() == 3) // 3 triangle indices, nothing to do:
            {
                objmesh.tvi.push_back(
                                      {std::get<0>(face_data)[0], std::get<0>(face_data)[1], std::get<0>(face_data)[2]});
                
                objmesh.tci.push_back(
                                      {std::get<1>(face_data)[0], std::get<1>(face_data)[1], std::get<1>(face_data)[2]});
            }
            // If their sizes are 4, we convert the quad to two triangles:
            // Actually I think MeshLab does the same, it shows the FaceWarehouse number of "Faces" as twice the "f" entries in the obj.
            else if (std::get<0>(face_data).size() == 4)
            {
                // Just create two faces with (quad[0], quad[1], quad[2]) and (quad[0], quad[2], quad[3]).
                objmesh.tvi.push_back(
                                      {std::get<0>(face_data)[0], std::get<0>(face_data)[1], std::get<0>(face_data)[2]});
                objmesh.tci.push_back(
                                      {std::get<1>(face_data)[0], std::get<1>(face_data)[1], std::get<1>(face_data)[2]});
                
                objmesh.tvi.push_back(
                                      {std::get<0>(face_data)[0], std::get<0>(face_data)[2], std::get<0>(face_data)[3]});
                objmesh.tci.push_back(
                                      {std::get<1>(face_data)[0], std::get<1>(face_data)[2], std::get<1>(face_data)[3]});
            }
            // We don't handle normal_indices for now.
        }
        // There can be other stuff in obj's like materials, named objects, etc., which are not handled here.
    }
    return objmesh;
}

#endif

