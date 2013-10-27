/*
 * Command.h
 *
 *  Created on: 27 Oct 2013
 *      Author: ensab
 */

#ifndef COMMAND_H_
#define COMMAND_H_

#define COMMAND_LENGTH 100

class CommandBuffer
{
  public:
    CommandBuffer();
    void Init();
    bool Put(char c);
    bool Seen(char c);
    float GetFValue();
    int GetIValue();
    long GetLValue();
    char* GetString();
    char* Buffer();
    bool Finished();
    void SetFinished(bool f);

  private:

    char gcodeBuffer[COMMAND_LENGTH];
    int gcodePointer;
    int readPointer;
    bool inComment;
    bool finished;
};

// Get an Int after a G Code letter

inline int CommandBuffer::GetIValue()
{
  return (int)GetLValue();
}

inline char* CommandBuffer::Buffer()
{
  return gcodeBuffer;
}

inline bool CommandBuffer::Finished()
{
  return finished;
}

inline void CommandBuffer::SetFinished(bool f)
{
  finished = f;
}



#endif /* COMMAND_H_ */
