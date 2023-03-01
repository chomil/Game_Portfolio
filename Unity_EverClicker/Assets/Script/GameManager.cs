using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class GameManager : MonoBehaviour
{
    public static GameManager instance = null;

    public Camera mainCamera;

    public Text starText;
    public Text moonText;


    public List<GameObject> monObjectList = new List<GameObject>();

    //Saved
    [SerializeField] private SaveData saveData = null;
    //Saved End

    public Sprite[] monSpriteList;
    public string[] monNameList;
    public int[] monUnlockPriceList;
    public int[] monSellPriceList;

    public int[] upgradeNumStarPriceList;
    public int[] upgradeTimeMoonPriceList;


    public int[] skillStarPriceList;
    public int[] skillExpPriceList;

    public RuntimeAnimatorController[] levelAc;


    public GameObject monPrefab;

    public SaveData GetSaveData()
    {
        return saveData;
    }

    public int MonsterKindNum()
    {
        int[] monId = new int[10];
        int monKind = 0;
        for(int i=0;i<monObjectList.Count;i++)
        {
            MonsterInfo monInfo = monObjectList[i].GetComponent<Monster>().GetMonsterInfo();
            if(monId[monInfo.id]==0)
            {
                monId[monInfo.id]++;
                monKind++;
            }
        }
        return monKind;
    }

    public void CreateMonster(MonsterInfo monsterInfo)
    {
        GameObject newMon = Instantiate(monPrefab, Vector3.zero, Quaternion.identity);
        newMon.GetComponent<Monster>().SetMonsterInfo(monsterInfo);
        newMon.GetComponent<Monster>().SetPositionRandom();
        newMon.GetComponent<SpriteRenderer>().sprite = GameManager.instance.monSpriteList[monsterInfo.id];
        newMon.name = GameManager.instance.monNameList[monsterInfo.id];
        monObjectList.Add(newMon);
    }

    public void ChangeAc(Animator anim, int level)
    {
        anim.runtimeAnimatorController = levelAc[level - 1];
    }

    public void AddStarPoint(int starPoint)
    {
        saveData.StarPoint = Mathf.Min(saveData.StarPoint + starPoint, 999999999);
    }

    public void Sell(GameObject monsterObject)
    {
        MonsterInfo mon = monsterObject.GetComponent<Monster>().GetMonsterInfo();
        int sellPrice = monSellPriceList[mon.id] * mon.level;
        saveData.MoonPoint = Mathf.Min(saveData.MoonPoint + sellPrice, 999999999);
        monObjectList.Remove(monsterObject);
        GameManager.instance.GameSave();
    }

    public Vector3 GetMousePos()
    {
        Vector3 clickPos = mainCamera.ScreenToWorldPoint(Input.mousePosition);
        clickPos.z = clickPos.y;
        return clickPos;
    }

    private void Awake()
    {
        if (instance == null)
        {
            instance = this;
            DontDestroyOnLoad(gameObject);
        }
        else
        {
            if (instance != this)
                Destroy(this.gameObject);
        }

    }

    void Start()
    {
        Debug.Log("GameManager");

        saveData = DataManager.Load();
        for (int i = 0; i < saveData.MonInfoJsonList.Count; i++)
        {
            MonsterInfo Info = JsonUtility.FromJson<MonsterInfo>(saveData.MonInfoJsonList[i]);
            CreateMonster(Info);
        }

        SoundManager.instance.UpdateVol();

        StartCoroutine("AutoProduceStar");
    }

    public void GameExit()
    {
#if UNITY_EDITOR
        UnityEditor.EditorApplication.isPlaying = false;
#else
        Application.Quit(); // 어플리케이션 종료
#endif
    }



    IEnumerator AutoProduceStar()
    {
        while (true)
        {
            float timeAuto = 10 - ((saveData.upgradeTimeLevel - 1) * 2);
            timeAuto = (timeAuto < 2) ? 2 : timeAuto;
            yield return new WaitForSeconds(timeAuto);
            int allStar = 0;

            int starLevel = GameManager.instance.GetSaveData().starSkillLevel;
            int monKind = GameManager.instance.MonsterKindNum();

            for (int i = 0; i < monObjectList.Count; i++)
            {
                MonsterInfo monInfo = monObjectList[i].GetComponent<Monster>().GetMonsterInfo();
                allStar += monInfo.starPerClick * monInfo.level * Mathf.Min(starLevel, monKind);
            }
            saveData.StarPoint = Mathf.Min(saveData.StarPoint + allStar, 999999999);
            GameSave(); //n초마다 자동저장
        }

    }

    // Update is called once per frame
    void Update()
    {


    }

    void LateUpdate()
    {
        starText.text = string.Format("{0:n0}", saveData.StarPoint);
        moonText.text = string.Format("{0:n0}", saveData.MoonPoint);
    }

    private void OnApplicationQuit()
    {
        GameSave();
    }

    public void GameSave()
    {
        saveData.MonInfoJsonList.Clear();
        for (int i = 0; i < monObjectList.Count; i++)
        {
            MonsterInfo info = monObjectList[i].GetComponent<Monster>().GetMonsterInfo();
            saveData.MonInfoJsonList.Add(info.MonsterInfoToJson());
        }
        DataManager.Save(saveData);
    }

    public void UnlockUpdate(int index, bool isUnlock)
    {
        saveData.MonUnlockList[index] = isUnlock;
    }

}
