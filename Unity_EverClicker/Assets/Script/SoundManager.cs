using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class SoundManager : MonoBehaviour
{

    public static SoundManager instance = null;

    public AudioSource BGM;
    public AudioSource SFX;

    public Slider bgmSlider;
    public Slider sfxSlider;

    public enum SFX_NAME
    {
        Button,Buy,Clear,Fail,Grow,Pause_In,Pause_Out,Sell,Touch,Unlock
    }
    public AudioClip[] sfxClips;

    void Awake()
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


    public void PlaySound(SFX_NAME name)
    {
        SFX.clip = sfxClips[(int)name];
        SFX.Play();
    }

    public void SlideBgmVol()
    {
        SaveData data = GameManager.instance.GetSaveData();
        data.bgmVol = bgmSlider.value;
        BGM.volume = data.bgmVol / 2;
    }
    public void SlideSfxVol()
    {
        SaveData data = GameManager.instance.GetSaveData();
        data.sfxVol = sfxSlider.value;
        SFX.volume = data.sfxVol;
    }

    public void UpdateVol()
    {
        SaveData data = GameManager.instance.GetSaveData();
        BGM.volume = data.bgmVol / 2;
        SFX.volume = data.sfxVol;
        bgmSlider.value = data.bgmVol;
        sfxSlider.value = data.sfxVol;
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
