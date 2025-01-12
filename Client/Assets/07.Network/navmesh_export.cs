﻿using System.IO;
using System.Text;
//using UnityEditor;
using UnityEngine.SceneManagement;
using UnityEngine;
using UnityEngine.AI;

//navmesh Export
public class NavMeshExport : MonoBehaviour
{
    /*
    private GameObject _testMap;

    [MenuItem("Tools/Export NavMesh Data")]
    private static void Export()
    {
        NavMeshTriangulation triangulatedNavMesh = NavMesh.CalculateTriangulation();

        Mesh mesh = new Mesh();
        mesh.name = "_NavMesh";
        mesh.vertices = triangulatedNavMesh.vertices;
        mesh.triangles = triangulatedNavMesh.indices;

        string baseName = SceneManager.GetActiveScene().name;
        string fileName = Application.dataPath + "/navmesh/" + baseName + ".navmesh";
        ExportNavmesh(mesh, fileName);

        AssetDatabase.Refresh();
        string assetName = fileName.Replace(Application.dataPath, "Assets");
        GameObject navMesh = Instantiate(AssetDatabase.LoadAssetAtPath<GameObject>(assetName));
        navMesh.name = baseName;
        ExportNavData(navMesh);
        Debug.Log("추출 완료：" + baseName);
        AssetDatabase.Refresh();
    }


    private static string ParseMesh(Mesh mesh)
    {
        StringBuilder sb = new StringBuilder();

        sb.Append("g ").Append(mesh.name).Append("\n");
        foreach (Vector3 v in mesh.vertices)
        {
            sb.Append(string.Format("v {0} {1} {2}\n", v.x, v.y, v.z));
        }
        sb.Append("\n");
        foreach (Vector3 v in mesh.normals)
        {
            sb.Append(string.Format("vn {0} {1} {2}\n", v.x, v.y, v.z));
        }
        sb.Append("\n");
        foreach (Vector3 v in mesh.uv)
        {
            sb.Append(string.Format("vt {0} {1}\n", v.x, v.y));
        }
        for (int m = 0; m < mesh.subMeshCount; m++)
        {
            sb.Append("\n");

            int[] triangles = mesh.GetTriangles(m);
            for (int i = 0; i < triangles.Length; i += 3)
            {
                sb.Append(string.Format("f {0}/{0}/{0} {1}/{1}/{1} {2}/{2}/{2}\n", triangles[i] + 1, triangles[i + 1] + 1, triangles[i + 2] + 1));
            }
        }
        return sb.ToString();
    }

    private static void ExportNavmesh(Mesh mesh, string filename)
    {
        using (StreamWriter sw = new StreamWriter(filename))
        {
            sw.Write(ParseMesh(mesh));
        }
    }

    private static void ExportNavData(GameObject obj)
    {
        //Vector3[] localVectors = obj.transform.Find("_NavMesh").GetComponent<MeshFilter>().sharedMesh.vertices;
        //int[] triangles = obj.transform.Find("_NavMesh").GetComponent<MeshFilter>().sharedMesh.triangles;
        ////메쉬의 로컬 좌표를 월드 좌표로 변환합니다.
        //Vector3[] worldVectors = new Vector3[localVectors.Length];
        //for (int i = 0; i < localVectors.Length; i++)
        //{
        //    Vector3 pos = obj.transform.TransformPoint(localVectors[i]);
        //    worldVectors[i] = pos;
        //}
        //StringBuilder sb = new StringBuilder();
        //sb.Append("local nav = {\n");
        //for (int i = 0; i < triangles.Length; i += 3)
        //{
        //    sb.AppendFormat("\t{{{0},{1},{2}}},\n", _VectorToLua(worldVectors[triangles[i]]), _VectorToLua(worldVectors[triangles[i + 1]]), _VectorToLua(worldVectors[triangles[i + 2]]));
        //}
        //sb.Append("}\n");
        //sb.Append("return nav");
        //using (StreamWriter sw = new StreamWriter(Application.dataPath + "/navmesh/" + obj.name + ".lua"))
        //{
        //    sw.Write(sb.ToString());
        //}
        //DestroyImmediate(obj);
    }

    private static string _VectorToLua(Vector3 vec)
    {
        return string.Format("{{{0},{1},{2}}}", vec.x, vec.y, vec.z);
    }

    //점이 삼각형 내에 있는지 결정합니다.
    public static bool IsInside(Vector3 A, Vector3 B, Vector3 C, Vector3 P)
    {
        Vector3 v0 = C - A;
        Vector3 v1 = B - A;
        Vector3 v2 = P - A;

        float dot00 = Vector3.Dot(v0, v0);
        float dot01 = Vector3.Dot(v0, v1);
        float dot02 = Vector3.Dot(v0, v2);
        float dot11 = Vector3.Dot(v1, v1);
        float dot12 = Vector3.Dot(v1, v2);

        float inverDeno = 1 / (dot00 * dot11 - dot01 * dot01);

        float u = (dot11 * dot02 - dot01 * dot12) * inverDeno;
        if (u < 0 || u > 1) // if u out of range, return directly
        {
            return false;
        }

        float v = (dot00 * dot12 - dot01 * dot02) * inverDeno;
        if (v < 0 || v > 1) // if v out of range, return directly
        {
            return false;
        }

        return u + v <= 1;

    }
    */
}