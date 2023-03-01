using System.Collections.Generic;
using UnityEngine;
using System.IO;
using System.Security.Cryptography;

public class DataManager : MonoBehaviour
{
    private static readonly string privateKey = "Crypto0Key1Ever2Clicker3";

    public static void Save(SaveData sd)
    {
        string jsonString = DataToJson(sd);
        string encryptString = Encrypt(jsonString);
        SaveFile(encryptString);
    }

    public static SaveData Load()
    {
        //������ �����ϴ������� üũ.
        if (!File.Exists(GetPath()))
        {
            Debug.Log("���̺� ������ �������� ����.");
            List<bool> unlockList = new List<bool>();
            for (int i = 0; i < GameManager.instance.monNameList.Length; i++)
            {
                unlockList.Add(false);
            }
            SaveData saveData = new SaveData(100, 100, 1, 1, unlockList, new List<string>(),0.5f,0.5f);
            return saveData;
        }

        string encryptData = LoadFile(GetPath());
        string decryptData = Decrypt(encryptData);


        SaveData saved = JsonToData(decryptData);
        return saved;
    }

    ////////////////////////////////////////////////////////////////////////////////////////

    //���̺� �����͸� json string���� ��ȯ
    static string DataToJson(SaveData sd)
    {
        string jsonData = JsonUtility.ToJson(sd);
        return jsonData;
    }

    //json string�� SaveData�� ��ȯ
    static SaveData JsonToData(string jsonData)
    {
        SaveData sd = JsonUtility.FromJson<SaveData>(jsonData);
        return sd;
    }

    ////////////////////////////////////////////////////////////////////////////////////////

    //json string�� ���Ϸ� ����
    static void SaveFile(string jsonData)
    {
        using (FileStream fs = new FileStream(GetPath(), FileMode.Create, FileAccess.Write))
        {
            //���Ϸ� ������ �� �ְ� ����Ʈȭ
            byte[] bytes = System.Text.Encoding.UTF8.GetBytes(jsonData);

            //bytes�� ���빰�� 0 ~ max ���̱��� fs�� ����
            fs.Write(bytes, 0, bytes.Length);
        }
    }

    //���� �ҷ�����
    static string LoadFile(string path)
    {
        using (FileStream fs = new FileStream(path, FileMode.Open, FileAccess.Read))
        {
            //������ ����Ʈȭ ���� �� ���� ������ ����
            byte[] bytes = new byte[(int)fs.Length];

            //���Ͻ�Ʈ������ ���� ����Ʈ ����
            fs.Read(bytes, 0, (int)fs.Length);

            //������ ����Ʈ�� json string���� ���ڵ�
            string jsonString = System.Text.Encoding.UTF8.GetString(bytes);
            return jsonString;
        }
    }

    ////////////////////////////////////////////////////////////////////////////////////////

    private static string Encrypt(string data)
    {
        byte[] bytes = System.Text.Encoding.UTF8.GetBytes(data);
        RijndaelManaged rm = CreateRijndaelManaged();
        ICryptoTransform ct = rm.CreateEncryptor();
        byte[] results = ct.TransformFinalBlock(bytes, 0, bytes.Length);
        return System.Convert.ToBase64String(results, 0, results.Length);

    }

    private static string Decrypt(string data)
    {
        byte[] bytes = System.Convert.FromBase64String(data);
        RijndaelManaged rm = CreateRijndaelManaged();
        ICryptoTransform ct = rm.CreateDecryptor();
        byte[] resultArray = ct.TransformFinalBlock(bytes, 0, bytes.Length);
        return System.Text.Encoding.UTF8.GetString(resultArray);
    }


    private static RijndaelManaged CreateRijndaelManaged()
    {
        byte[] keyArray = System.Text.Encoding.UTF8.GetBytes(privateKey);
        RijndaelManaged result = new RijndaelManaged();

        byte[] newKeysArray = new byte[16];
        System.Array.Copy(keyArray, 0, newKeysArray, 0, 16);

        result.Key = newKeysArray;
        result.Mode = CipherMode.ECB;
        result.Padding = PaddingMode.PKCS7;
        return result;
    }

    ////////////////////////////////////////////////////////////////////////////////////////

    //������ �ּҸ� ��ȯ
    static string GetPath()
    {
        return Path.Combine(Application.persistentDataPath, "savedata.dat");
    }

}
