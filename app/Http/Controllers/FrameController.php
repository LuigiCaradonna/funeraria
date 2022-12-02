<?php

namespace App\Http\Controllers;

use App\Models\Frame;
use Illuminate\Http\Request;
use Inertia\Inertia;

class FrameController extends Controller
{
    /**
     * Shows the Frame list
     * 
     * @param \Illuminate\Http\Request $request contains the eventual filters to customize the list
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the Frame page
     */
    public function index(Request $request)
    {
        $search = $request->input('search') === null ? ""    : $request->input('search');
        $field  = $request->input('field')  === null ? "name": $request->input('field');
        $order  = $request->input('order')  === null ? 'ASC' : ($request->input('order') === 'true' ? 'ASC' : 'DESC');

        $frames = Frame::query();

        if ( $search != "" ) 
        {
            $frames->where('name', 'like', "%{$search}%")->orderby($field, $order);
        }
        else 
        {
            $frames->orderby($field, $order);
        }

        return Inertia::render('Frames', [
            'frames' => $frames->paginate(20)->withQueryString(), // This works but VSCode doesn't know
            'filters' => $request->only(['search'])
        ]);
    }

    /**
     * Creates a new Frame
     * 
     * @param \Illuminate\Http\Request $request contains the data to create the Frame
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the Frames page
     */
    public function store(Request $request)
    {
        if ( $request->input('id') != 0 )
            return redirect()->route('frames')->with('error', 'Creazione cornice non riuscita.');

        $validated = $request->validate([
            'name' => 'required'
        ]);

        if ( ! Frame::create($validated) )
        {
            return redirect()->route('frames')->with('error', 'Creazione cornice non riuscita.');
        }

        return redirect()->route('frames')->with('success', 'Cornice creata con successo.');
    }

    /**
     * Updates a Frame given its id
     * 
     * @param \Illuminate\Http\Request $request contains the data to update the Frame
     * @param int $id id of the Frame to update
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the Frames page
     */
    public function edit(Request $request, $id)
    {
        if ( $id == 0 )
            return redirect()->route('frames')->with('error', 'Cornice non trovata.');

        $request->validate([
            'name' => 'required'
        ]);

        $frame = Frame::find($id);

        if ( ! $frame )
        {
            return redirect()->route('frames')->with('error', 'Cornice non trovata.');
        }

        $frame->name = $request->input('name');

        if ( ! $frame->save() )
            return redirect()->route('frames')->with('error', 'Modifica cornice non riuscita.');
            
        return redirect()->route('frames')->with('success', 'Cornice modificata con successo.');
    }

    /**
     * Deletes a Frame given its id
     * 
     * @param int $id id of the Frame to delete
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the Frames page
     */
    public function delete($id)
    {
        if ( $id <= 0 || $id == null )
            return redirect()->route('frames')->with('error', 'Cornice non trovata.');

        // This is executed only if the validation succeedes
        $frame = Frame::find($id);

        if ( ! $frame )
        {
            return redirect()->route('frames')->with('error', 'Cornice non trovata.');
        }

        if ( ! $frame->delete() )
            return redirect()->route('frames')->with('error', 'Eliminazione cornice non riuscita.');
            
        return redirect()->route('frames')->with('success', 'Cornice eliminata con successo.');
    }
    
    /**
     * Deletes a set of Frames whose ids are conteined into the "ids" parameter coming from the request
     * 
     * @param \Illuminate\Http\Request $request containing the ids of the Frame to delete
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the Frame page
     */
    public function multiDelete(Request $request)
    {
        if ( Frame::destroy(collect($request->input('ids'))) )
            return redirect()->route('frames')->with('success', 'Cornici eliminate con successo.');

        return redirect()->route('frames')->with('error', 'Eliminazione cornici non riuscita.');
    }
}
