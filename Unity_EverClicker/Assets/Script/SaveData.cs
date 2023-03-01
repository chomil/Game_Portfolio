using System.Collections.Generic;
using UnityEngine;


[System.Serializable]
public class SaveData
{
    public int StarPoint;
    public int MoonPoint;

    public int upgradeNumLevel;
    public int upgradeTimeLevel;

    public int starSkillLevel;
    public int expSkillLevel;

    public List<bool> MonUnlockList;
    public List<string> MonInfoJsonList;

    public float bgmVol;
    public float sfxVol;

    //»ý¼ºÀÚ
    public SaveData(int StarPoint, int MoonPoint, int upgradeNumLevel, int upgradeTimeLevel, 
                    List<bool> MonUnlockList, List<string> MonInfoJsonList, float bgmVol, float sfxVol)
    {
        this.StarPoint = StarPoint;
        this.MoonPoint = MoonPoint;

        this.upgradeNumLevel = upgradeNumLevel;
        this.upgradeTimeLevel = upgradeTimeLevel;

        this.MonUnlockList = new List<bool>();

        for (int i = 0; i < MonUnlockList.Count; i++)
        {
            this.MonUnlockList.Add(MonUnlockList[i]);
        }

        this.MonInfoJsonList = new List<string>();
        for (int i = 0; i < MonInfoJsonList.Count; i++)
        {
            this.MonInfoJsonList.Add(MonInfoJsonList[i]);
        }

        this.bgmVol = bgmVol;
        this.sfxVol = sfxVol;
    }
}
