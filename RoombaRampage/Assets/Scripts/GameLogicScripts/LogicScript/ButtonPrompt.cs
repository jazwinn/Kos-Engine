using System;

public class ButtonPrompt : ScriptBase
{
    #region Entity ID
    private uint EntityID;

    public override void Awake(uint id)
    {
        EntityID = id;
        howToPlayTextureController = "img_userManualController.png";
        howToPlayTextureMouse = "img_userManual.png";
    }
    #endregion

    public int promptNo;
    private AnimationComponent animComp;
    private string howToPlayTextureController;
    private string howToPlayTextureMouse;

    //image
    private string startingSprite;
    private int startingLayer;
    private Vector3 startingColor;
    private float startingAlpha;

    private bool controllerCheck;

    //text
    private string controllerText = "Y/B";
    private string mouseText = "Y/N";

    private string confirmationText;
    private string confirmationFontFileName;
    private int confirmationFontLayer;
    private float confirmationFontSize;
    private Vector3 confirmationFontColor;

    public override void Start()
    {
        animComp = Component.Get<AnimationComponent>(EntityID);
        InternalCall.m_InternalGetSpriteComponent(EntityID, out startingSprite, out startingLayer, out startingColor, out startingAlpha);
        InternalCall.m_InternalGetTextComponent(EntityID, out confirmationText, out confirmationFontFileName, out confirmationFontLayer, out confirmationFontSize,out confirmationFontColor);
    }

    public override void Update()
    {
        controllerCheck = InternalCall.m_InternalCallIsControllerPresent();

        if (controllerCheck == true)
        {
            switch (promptNo)
            {
                case 1:
                    animComp.m_frameNumber = 1;
                    Component.Set<AnimationComponent>(EntityID, animComp);
                    break;

                case 2:

                    InternalCall.m_InternalSetSpriteComponent(EntityID, howToPlayTextureController, startingLayer, startingColor, startingAlpha);
                    break;

                case 3:
                    InternalCall.m_InternalSetTextComponent(EntityID, controllerText, confirmationFontFileName,  confirmationFontLayer,  confirmationFontSize,  confirmationFontColor);
                    break;

                default:
                    break;

            }
        }
        else
        {
            switch (promptNo)
            {
                case 1:
                    animComp.m_frameNumber = 0;
                    Component.Set<AnimationComponent>(EntityID, animComp);
                    break;

                case 2:
                    InternalCall.m_InternalSetSpriteComponent(EntityID, howToPlayTextureMouse, startingLayer, startingColor, startingAlpha);
                    break;

                case 3:
                    InternalCall.m_InternalSetTextComponent(EntityID, mouseText, confirmationFontFileName, confirmationFontLayer, confirmationFontSize, confirmationFontColor);
                    break;

                default:
                    break;

            }
        }

    }
}